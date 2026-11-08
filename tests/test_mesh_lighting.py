"""Check the remaining mesh-lighting instruction differences against the target.

These are static output checks, not a rendering/runtime test. Public CI skips
this class because it has neither the original executable nor native outputs.
"""
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from comparisons import function_bytes, inspect_comparison, load_comparisons
from formats import Elf32


class MeshLightingComparisons(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.names = ['compare_eagl_msh_' + name + '_lit' for name in
                     ('color_slots', 'texture1', 'texture2', 'texture1_lightmap')]
        if not all((ROOT/'build/comparisons'/name/'compiled.elf').exists() for name in cls.names):
            raise unittest.SkipTest('Run comparisons.py with the mesh-lighting sources first')
        from audit import load_target
        _, cls.original = load_target()
        cls.units = [u for u in load_comparisons(ROOT) if u['id'] in cls.names]

    def test_complete_outputs_and_target_extent(self):
        self.assertEqual(len(self.units), 4)
        self.assertEqual(sum(f['size'] for u in self.units for f in u['functions']), 13380)
        for unit in self.units:
            with self.subTest(unit=unit['id']):
                work = ROOT/'build/comparisons'/unit['id']
                linked = Elf32((work/'compiled.elf').read_bytes())
                records = inspect_comparison(self.original, Elf32((work/'compiled.o').read_bytes()), linked, unit)
                self.assertEqual(len(records), 1)
                self.assertEqual(records[0]['size'], records[0]['compiled_size'])
                target = function_bytes(self.original, unit['functions'][0])
                native = function_bytes(linked, unit['functions'][0])
                self.assertEqual(target[:4], native[:4])  # Identical stack allocation.

    def test_only_independent_argument_setup_swaps_remain(self):
        def assignment(word):
            # Only addi (without CR side effects) and mr = or rD,rS,rS.
            if word >> 26 == 14:
                dest, source = (word >> 21) & 31, (word >> 16) & 31
                return dest, {source} if source else set()
            self.assertEqual(word >> 26, 31)
            self.assertEqual((word >> 1) & 1023, 444)
            self.assertEqual(word & 1, 0)
            self.assertEqual((word >> 21) & 31, (word >> 11) & 31)
            return (word >> 16) & 31, {(word >> 21) & 31}

        for unit in self.units:
            with self.subTest(unit=unit['id']):
                f = unit['functions'][0]
                linked = Elf32((ROOT/'build/comparisons'/unit['id']/'compiled.elf').read_bytes())
                original = function_bytes(self.original, f)
                compiled = function_bytes(linked, f)
                self.assertEqual(len(compiled), len(original))
                a = struct.unpack('>%dI' % (len(original)//4), original)
                b = struct.unpack('>%dI' % (len(compiled)//4), compiled)
                differences = [i for i, (x, y) in enumerate(zip(a, b)) if x != y]
                branch_entries = set()
                for index, word in enumerate(a):
                    op = word >> 26
                    if op in (16, 18):
                        bits = 16 if op == 16 else 26
                        disp = word & ((1 << bits)-4)
                        if disp & (1 << (bits-1)):
                            disp -= 1 << bits
                        destination = disp if word & 2 else int(f['address'],16) + index*4 + disp
                        branch_entries.add(destination)
                    elif op == 19 and ((word >> 1) & 1023) in (16, 528):
                        self.assertEqual(word, 0x4e800020)  # No computed entries into a swap.
                self.assertEqual(len(differences) % 2, 0)
                for first, second in zip(differences[::2], differences[1::2]):
                    self.assertEqual(second, first+1)
                    self.assertEqual((a[first], a[second]), (b[second], b[first]))
                    dst1, reads1 = assignment(a[first])
                    dst2, reads2 = assignment(a[second])
                    self.assertIn(dst1, range(3, 11))
                    self.assertIn(dst2, range(3, 11))
                    self.assertNotEqual(dst1, dst2)
                    self.assertNotIn(dst1, reads2)
                    self.assertNotIn(dst2, reads1)
                    self.assertNotIn(int(f['address'],16)+second*4, branch_entries)
                    self.assertEqual(a[second+1] >> 26, 18)
                    self.assertEqual(a[second+1] & 1, 1)  # Same following direct call.


if __name__ == '__main__':
    unittest.main()
