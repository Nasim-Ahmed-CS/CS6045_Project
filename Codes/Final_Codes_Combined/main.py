import visualization as vs
import kruskal as ks
import prims as ps
import os
file_name = "input.txt"

print("---------------------------------------------------------------------")

print("Showing the visualization")
print("---------------------------------------------------------------------")

print("Showing the visualization ended")
print("---------------------------------------------------------------------")

if file_name in os.listdir():
    vs.process_input_file(file_name)
else:
    print(f"File '{file_name}' not found in the current directory: {os.getcwd()}")


print("---------------------------------------------------------------------")
print("Output for the Kruskals Algorithm")
print("---------------------------------------------------------------------")

try:
    ks.processInputFile("input.txt")
except FileNotFoundError:
    print(f"Error: The file '{"input.txt"}' does not exist in the current directory.")
print("---------------------------------------------------------------------")



print("---------------------------------------------------------------------")
print("Output for the Prims Algorithm")
print("---------------------------------------------------------------------")

try:
    ps.processInputFile("input.txt")
except FileNotFoundError:
    print(f"Error: The file '{"input.txt"}' does not exist in the current directory.")
print("---------------------------------------------------------------------")
