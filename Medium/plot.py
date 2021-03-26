import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def p(x):
    return np.sqrt(4.5*(x-1) + np.power(x-1, 2) - 4*np.power(x-1, 3) - np.power(x-1, 4) + 3);
    # return np.sqrt(x + np.power(x, 2) - np.power(x, 3) + 2)

def q(x):
    return 1.0/(b - a)

# filename = "data_1.txt"
# a, b = -2.0, 2.0

filename = "data_2.txt"
a, b = -3.0, 2.2

x_uniform = np.random.uniform(a, b, 10000)
k = max(p(x_uniform) / q(x_uniform))
C = (b - a) * np.mean(p(x_uniform))
print("Max value: ", k)
print("Normalization constant: ", C)

def read_data_from_file(filename):
    ret_arr = []
    with open(filename) as file:
        ret_arr = file.readlines()

    ret_arr = np.array(ret_arr, dtype=float)
    return ret_arr


if __name__ == '__main__':
    fig, ax = plt.subplots(1, 1)
    x  = np.linspace(a, b, 1000)
    ax.plot(x, p(x)/C, label="Normalized target pdf")
    ax.plot(x, np.full(len(x), (k/C)*q(0)), label="Proposal pdf (Uniform)")

    ret_arr = read_data_from_file(filename)
    sns.histplot(ret_arr, ax=ax, bins=30, stat='density', label="Histogram of sampled points")

    ax.legend(loc="upper right")

    # ax.set_title("Unnormalized target pdf: ${\sqrt{x + x^2 - x^3 + 2}}$")
    ax.set_title("Unnormalized target pdf: ${\sqrt{4.5(x-1) + (x-1)^2 - 4(x-1)^3 - (x-1)^4 + 3}}$")
    ax.set_xlabel(f"Support: ({a}, {b})")
    plt.show()

