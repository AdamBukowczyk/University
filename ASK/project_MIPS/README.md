Asembler dla architektury MIPS

Przygotował: Adam Bukowczyk

Wymagania: Python 3.6

Sposób uruchomienia:
1. Linux:
python3 mipsasm.py tests/<nazwa_testu>
2. Windows (jeśli podepniemy kompilator Python do zmiennych środowiskowych)
python mipsasm.py tests/<nazwa_testu>

Wynik zostanie wypisany na standardowe wyjście (w konsoli).
Instrukcje w plikach testowych powinny być podane w następującej formie:
1.Nazwa instrukcji
2.Pojedynczy tab
3.Operandy odzielone przecinakami
4.(Opcjonalnie)Komentarz po znaku #

Wzorzec instrukcji:
instrukcja \t op1,op2,op3 # komentarz

