import sys
from pathlib import Path
from test.cunit import SRC
from test.cunit import CModule


CFLAGS = ["-g", "-fprofile-arcs", "-ftest-coverage", "-fPIC"]

EXTRA_SOURCES = [
    SRC / "argparse.c",
    SRC / "logging.c",
]

sys.modules[__name__] = CModule.compile(
    SRC / Path(__file__).stem, cflags=CFLAGS, extra_sources=EXTRA_SOURCES
)
