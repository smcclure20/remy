

output_file = open("./output/new_config_shares.csv", "w")
output_file.write("Rate,Senders,Share\n")

# rates = list(range(1, 100, 10))
# srcs = list(range(1, 50, 5))
rates = list(range(20, 42, 5))
rates = [rate/10 for rate in rates]
srcs = list(range(1, 16, 1))

for rate in rates:
    for src in srcs:
        output_file.write("{},{},{}\n".format(rate, src, rate/src))

output_file.close()