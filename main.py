import sys
import os

# Dodaj cpp_modules do ścieżki importu
sys.path.append(os.path.join(os.path.dirname(__file__), 'cpp_modules'))

import main_cpp

print(main_cpp.dodaj(2, 3))  # powinno wypisać 5
