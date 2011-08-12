import os
import sys
old_path = sys.path
sys.path = [os.path.join(os.path.dirname(__file__), '..', 'Debug')]
from cvpush import *
sys.path = old_path

