import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def p(x):
    return np.sqrt(x + np.power(x, 2) - np.power(x, 3) + 2)

def q(x):
    return 1.0/(b - a)

filename = "data_1.txt"
a, b = -2.0, 2.0
x_uniform = np.random.uniform(a, b, int((b-a)*10000))

C = (b - a) * np.mean(p(x_uniform))
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

    ret_arr = read_data_from_file(filename)
    sns.histplot(ret_arr, bins=30, ax=ax, stat='density', label="Histogram of sampled points")

    ax.legend(loc="upper center")
    ax.set_title("Unnormalized target pdf: ${\sqrt{x + x^2 - x^3 + 2}}$")
    ax.set_xlabel("Support: (-2, 2)")
    plt.show()

