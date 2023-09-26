import subprocess
import sys

FILE = "./A1"
SOURCES = "".join([sys.argv[i] + " " for i in range(1, len(sys.argv))])
TESTS = ["car-T1.arff", "iris-T1.arff", "netlog-T1.arff", "formacao-T2.arff", "formacao-T3.arff", "formacao-T4.arff", "formacao-T5.arff", "formacao-T6.arff", "formacao-INEX.arff"]
RESULTS = []

print("\n##================== TESTE: Compilação ==================##\n")
try:
	res_compile = subprocess.check_output("gcc -Wall " + SOURCES + " -o A1", shell=True)
except:
	print("ERRO FATAL: O PROGRAMA NÃO COMPILOU!")
	exit(1)
print("\n##===========================================================##\n")

for t_file in TESTS:
	result = None
	try:
		print("\n##================== TESTE: " + t_file + " ==================##\n")
		try:
			result = subprocess.check_output(FILE + " -i " + t_file + " -p", shell=True, timeout=3)
		except subprocess.CalledProcessError as grepexc:                                                                                                   
			print("Código de erro retornado pelo programa: ", grepexc.returncode)
		except Exception as e:
			print("Parada por timeout! O teste não foi concluído.")
		print(str(result, 'utf-8', 'ignore'))
		print("\n##===========================================================##\n")
	except Exception as e:
		if result != None:
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
			print("\n##===========================================================##\n")
		else:
			print("\n##===========================================================##\n")
		continue

print("\n##================== TESTE: VALGRIND ==================##\n")
try:
	result = subprocess.check_output("valgrind " + FILE + " -i " + TESTS[0], shell=True)
	print(str(res_compile, 'utf-8', 'ignore'))
except:
	print(result)
print("\n##===========================================================##\n")