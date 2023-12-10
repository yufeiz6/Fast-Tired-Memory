import matplotlib.pyplot as plt

# Data for the graph
localities = ['0.2', '0.5', '0.9']
subpage_rates = [0.9313, 0.957, 0.9791]
hugepage_rates = [0.5694, 0.5578, 0.5597]

x = range(len(localities))  # the label locations

# Creating the plot
plt.figure(figsize=(10, 6))

# Plotting Subpage and Hugepage data
subpage_bars = plt.bar([i - 0.2 for i in x], subpage_rates, width=0.4, label='Subpage', color='skyblue')
hugepage_bars = plt.bar([i + 0.2 for i in x], hugepage_rates, width=0.4, label='Hugepage', color='orange')

# Function to add labels on top of each bar
def add_labels(bars):
    for bar in bars:
        height = bar.get_height()
        plt.annotate(f'{height:.4f}',
                     xy=(bar.get_x() + bar.get_width() / 2, height),
                     xytext=(0, 3),  # 3 points vertical offset
                     textcoords="offset points",
                     ha='center', va='bottom')

# Adding labels to the bars
add_labels(subpage_bars)
add_labels(hugepage_bars)

# Adding labels and title
plt.xlabel('Locality')
plt.ylabel('Cache Hit Rate')
plt.title('Cache Hit Rates by Locality and Page Type')
plt.xticks(x, localities)
plt.legend()

# Display the graph
plt.show()
