import matplotlib
matplotlib.use('Agg')
import os
import time
import subprocess
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
C_PROGRAM_NAME = "./svd_compress" #i am making sure that the python program can itself run the C so that it cna import values and plot graphs 
if os.name == 'nt':
    C_PROGRAM_NAME = ".\\svd_compress.exe"
SOURCE_IMAGES_FOR_NUMPY = {
    'greyscale.pgm': 'grayscale',
}
K_VALUES = [5, 20, 50, 100]
TIMEOUT_SECONDS = 600
def run_c_program():
    print(f"doing the svd algo now")
    start_time = time.time()
    try:
        result = subprocess.run(
            [C_PROGRAM_NAME],
            check=True,
            timeout=TIMEOUT_SECONDS,
            capture_output=True,
            text=True
        )
        end_time = time.time()
        print(result.stdout)
        print(f"code done in {end_time - start_time:.2f} seconds.")
        return True
    except subprocess.TimeoutExpired:
        print(f"took lot of time {TIMEOUT_SECONDS} seconds.")
        return False
    except subprocess.CalledProcessError as e:
        print("the shit crashed")
        return False
    except FileNotFoundError:
        return False
def plot_singular_values():
    print("\ndoing the plot now")
    try:
        plot_created = False
        for basename in SOURCE_IMAGES_FOR_NUMPY.keys():
            txt_filename = f"singular_values_{basename}.txt"
            if os.path.exists(txt_filename):
                plot_filename = f"singular_values_plot_{basename}.png"
                s_values = np.loadtxt(txt_filename)
                plt.figure(figsize=(10, 6))
                plt.plot(range(1, len(s_values) + 1), s_values, 'bo-')
                plt.title(f'Singular Value Decay for {basename}')
                plt.xlabel('k (Component Index)')
                plt.ylabel('Singular Value (Log Scale)')
                plt.yscale('log')
                plt.grid(True)
                plt.savefig(plot_filename)
                plt.close()
                print(f"plotted singular values to {plot_filename}")
                plot_created = True
        if not plot_created:
            print("No singular value text files were found to plot.")

    except Exception as e:
        print(f"An error occurred during plotting: {e}")

def run_comparison():
    print("\n doing the comparison")
    try:
        for filename, mode in SOURCE_IMAGES_FOR_NUMPY.items():
            if not os.path.exists(filename):
                print(f"Source image {filename} not found. Skipping comparison.")
                continue
            if mode == 'grayscale':
                img = Image.open(filename)
                A = np.array(img)
                print(f"Running NumPy SVD (k=100) on {filename}")
                start_time = time.time()
                U, S, Vh = np.linalg.svd(A, full_matrices=False)
                k = 100
                A_k = U[:, :k] @ np.diag(S[:k]) @ Vh[:k, :]
                end_time = time.time()
                print(f"NumPy SVD executed in {end_time - start_time:.6f} seconds.")
            
    except Exception as e:
        print(f"An error occurred during NumPy comparison: {e}")

def main():
    if run_c_program():
        plot_singular_values()
        run_comparison()
    else:
        print("\nC program failed. Skipping analysis.")

if __name__ == "__main__":
    main()
