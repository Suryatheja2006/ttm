import matplotlib.pyplot as plt

# Load data from out.txt
file_path = './out.txt'
data = []
with open(file_path, 'r') as f:
    for line in f:
        x, y = map(int, line.split())
        data.append((x, y))

# Separate x and y values
x_values, y_values = zip(*data)

# Create the plot
plt.figure(figsize=(10, 6))
plt.style.use('dark_background')

# Plot each segment with specified colors and increased point size
plt.scatter(x_values[:50], y_values[:50], color='darkgreen', s=50, label='>=16')
plt.scatter(x_values[50:100], y_values[50:100], color='lightgreen', s=50, label='>=10')
plt.scatter(x_values[100:150], y_values[100:150], color='yellow', s=50, label='>=4')
plt.scatter(x_values[150:], y_values[150:], color='white', s=50, label='<4')

# Add labels, grid, legend, and display the plot
plt.xlabel('LCS',fontsize=14, color='white')
plt.ylabel('distance as measured by simHash', fontsize=14, color='white')
plt.grid(True, color='gray', linestyle='--', linewidth=0.5)
plt.legend(facecolor='black', framealpha=0.8, fontsize=10)
plt.title('Scatter plot of distance vs lcs of 20 length samples', fontsize=16, color='white')
plt.savefig('scatter_plot_simHash.png', dpi=300, bbox_inches='tight')
plt.show()
