import os

cur_path = os.path.abspath(os.path.dirname(__file__))
ver_file = os.path.join(cur_path, "VERSION.txt")

if not os.path.exists(ver_file):
    __version__ = "dev"
else:
    with open(ver_file, "r") as f:
        __version__ = f.read().strip()
