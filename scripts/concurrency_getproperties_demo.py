#!/usr/bin/env python3
"""Video decoder concurrency demo with getProperties API.

Loads node definitions (codec, profile, maxLevel, dynamicRange) from
hfp-videodecoder.yaml and concurrency combinations from
hfp-videodecoder-concurrency.yaml.

  getProperties()  -> returns VideoCapabilities (available nodes)
  allocate(nodeId) -> selects a node, marks it allocated, prunes the graph
  free(nodeId)     -> releases a node, restores pruned edges

Usage:
  python concurrency_getproperties_demo.py [decoder.yaml] [concurrency.yaml]
"""

from __future__ import annotations

import os
import sys
from dataclasses import dataclass
from typing import Dict, List, Sequence, Set, Tuple

try:
    from ruamel.yaml import YAML as _YAML

    def _load_yaml(path: str) -> dict:
        with open(path, encoding="utf-8") as fh:
            return _YAML().load(fh)
except ImportError:
    import yaml  # type: ignore[import-untyped]

    def _load_yaml(path: str) -> dict:
        with open(path, encoding="utf-8") as fh:
            return yaml.safe_load(fh)


# ── Data model ───────────────────────────────────────────────────────────

@dataclass(frozen=True)
class VideoNode:
    node_id: str
    decoder: int
    codec: str
    profile: str
    max_level: str
    dynamic_range: Tuple[str, ...]

    @property
    def label(self) -> str:
        dr = ",".join(self.dynamic_range)
        return f"D{self.decoder}:{self.codec}/{self.profile}/{self.max_level} [{dr}]"


@dataclass
class VideoCapabilities:
    """Return type for getProperties – mirrors what MW would expose."""
    max_concurrent: int
    allocated: List[VideoNode]
    available: List[VideoNode]


# ── Graph allocator (MW side) ───────────────────────────────────────────

class ConcurrencyGraph:
    """Builds the concurrency graph from two VL static files."""

    def __init__(self, decoder_yaml: str, concurrency_yaml: str) -> None:
        # Load node definitions from hfp-videodecoder.yaml
        dec_raw = _load_yaml(decoder_yaml)
        dec_root = dec_raw["videodecoder"]

        self._nodes: Dict[str, VideoNode] = {}
        for cap_entry in dec_root["Capabilities"]:
            for decoder_key, decoder_data in cap_entry.items():
                decoder_idx = int(decoder_key)
                for codec_entry in decoder_data["codecCapabilities"]:
                    for codec_name, codec_data in codec_entry.items():
                        dr = tuple(codec_data["dynamicRange"])
                        for profile_entry in codec_data["profiles"]:
                            for profile_name, profile_data in profile_entry.items():
                                node_id = profile_data.get("nodeId")
                                if node_id is None:
                                    continue
                                node = VideoNode(
                                    node_id=str(node_id),
                                    decoder=decoder_idx,
                                    codec=str(codec_name),
                                    profile=str(profile_name),
                                    max_level=str(profile_data["maxLevel"]),
                                    dynamic_range=dr,
                                )
                                self._nodes[node.node_id] = node

        # Load concurrency policy
        conc_raw = _load_yaml(concurrency_yaml)
        conc_root = conc_raw["videodecoderconcurrency"]
        self._max_concurrent: int = int(conc_root["maxConcurrentInstances"])

        # Build allowed-pair set (bidirectional edges)
        self._allowed_pairs: Set[Tuple[str, str]] = set()
        for pair in conc_root["combinations"]:
            a, b = str(pair[0]), str(pair[1])
            self._allowed_pairs.add((a, b))
            self._allowed_pairs.add((b, a))

        self._allocated: List[str] = []

    # ── getProperties (called by Application Layer) ──────────────────

    def get_properties(self) -> VideoCapabilities:
        """Return current video capabilities: allocated nodes + available nodes."""
        available = self._feasible_nodes()
        return VideoCapabilities(
            max_concurrent=self._max_concurrent,
            allocated=[self._nodes[nid] for nid in self._allocated],
            available=available,
        )

    # ── allocate / free ──────────────────────────────────────────────

    def allocate(self, node_id: str) -> bool:
        """Select a node for playback. Returns True on success."""
        if node_id not in self._nodes:
            raise ValueError(f"Unknown node: {node_id}")
        if node_id in self._allocated:
            return False
        if len(self._allocated) >= self._max_concurrent:
            return False
        if not self._is_compatible(node_id):
            return False
        self._allocated.append(node_id)
        return True

    def free(self, node_id: str) -> bool:
        """Release a node after playback finishes. Returns True on success."""
        if node_id in self._allocated:
            self._allocated.remove(node_id)
            return True
        return False

    # ── Internal helpers ─────────────────────────────────────────────

    def _is_compatible(self, candidate: str) -> bool:
        cand = self._nodes[candidate]
        for sel_id in self._allocated:
            sel = self._nodes[sel_id]
            if sel.decoder == cand.decoder:
                return False
            if (sel_id, candidate) not in self._allowed_pairs:
                return False
            # dynamicRange must have at least one common format
            if not set(sel.dynamic_range) & set(cand.dynamic_range):
                return False
        return True

    def _feasible_nodes(self) -> List[VideoNode]:
        if len(self._allocated) >= self._max_concurrent:
            return []
        result = []
        for node in self._nodes.values():
            if node.node_id in self._allocated:
                continue
            if self._is_compatible(node.node_id):
                result.append(node)
        return result


# ── Pretty printing ──────────────────────────────────────────────────────

def print_capabilities(caps: VideoCapabilities) -> None:
    print(f"\n{'=' * 78}")
    print(f"VIDEO CAPABILITIES  (maxConcurrent={caps.max_concurrent})")
    print(f"{'=' * 78}")

    if caps.allocated:
        print("\nALLOCATED:")
        for node in caps.allocated:
            print(f"  [{node.node_id:28s}] {node.label}")
    else:
        print("\nALLOCATED: <none>")

    if caps.available:
        print(f"\nAVAILABLE ({len(caps.available)} nodes):")
        for node in caps.available:
            print(f"  {node.node_id:28s}  {node.label}")
    else:
        print("\nAVAILABLE: <none>")


def print_help() -> None:
    print("\nCommands:")
    print("  getProperties            show current video capabilities")
    print("  maxConcurrentInstances   show max concurrent decoder limit")
    print("  allocate <node_id>       select a node for playback")
    print("  free <node_id>           release a node after playback")
    print("  nodes                    list all node IDs")
    print("  quit                     exit")


# ── Main loop ────────────────────────────────────────────────────────────

def main() -> None:
    base = os.path.join(os.path.dirname(__file__), "..", "hfp-reference", "videodecoder")
    decoder_yaml = sys.argv[1] if len(sys.argv) > 1 else os.path.join(base, "hfp-videodecoder.yaml")
    concurrency_yaml = sys.argv[2] if len(sys.argv) > 2 else os.path.join(base, "hfp-videodecoder-concurrency.yaml")

    print(f"Nodes from:        {decoder_yaml}")
    print(f"Combinations from: {concurrency_yaml}")
    graph = ConcurrencyGraph(decoder_yaml, concurrency_yaml)
    print(f"Loaded {len(graph._nodes)} nodes, "
          f"{len(graph._allowed_pairs) // 2} combination edges, "
          f"maxConcurrentInstances={graph._max_concurrent}")

    print_help()

    # Show initial getProperties
    print_capabilities(graph.get_properties())

    while True:
        try:
            raw = input("\n> ").strip()
        except (EOFError, KeyboardInterrupt):
            break
        if not raw:
            continue

        cmd, *rest = raw.split(maxsplit=1)
        arg = rest[0] if rest else ""

        if cmd == "quit":
            break

        if cmd == "getProperties":
            print_capabilities(graph.get_properties())
            continue

        if cmd == "maxConcurrentInstances":
            print(f"maxConcurrentInstances: {graph._max_concurrent}")
            continue

        if cmd == "allocate":
            if not arg:
                print("Usage: allocate <node_id>")
                continue
            try:
                ok = graph.allocate(arg)
            except ValueError as exc:
                print(f"ERROR: {exc}")
                continue
            if ok:
                print(f"ALLOCATED: {arg}")
                print_capabilities(graph.get_properties())
            else:
                print(f"BLOCKED: {arg} cannot be allocated right now")
            continue

        if cmd == "free":
            if not arg:
                print("Usage: free <node_id>")
                continue
            if graph.free(arg):
                print(f"FREED: {arg}")
                print_capabilities(graph.get_properties())
            else:
                print(f"NOT ALLOCATED: {arg}")
            continue

        if cmd == "nodes":
            for nid in sorted(graph._nodes):
                print(f"  {nid}")
            continue

        print(f"Unknown command: {cmd}")
        print_help()

    print("\nFinal state:")
    print_capabilities(graph.get_properties())


if __name__ == "__main__":
    main()
