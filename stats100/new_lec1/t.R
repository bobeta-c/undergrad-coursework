## CLT and LLN
# Number of trials
trials <- 10000

# Initialize vectors to store the results
mean_u <- numeric(trials)
clt_u <- numeric(trials)

# Number of observations (change this to see different effects)
n <- 30

# Simulation
for (i in 1:trials) {
  # Generate n uniform random numbers
  U <- runif(n, min = 0, max = 1)

  # Calculate the mean
  mean_u[i] <- mean(U)

  # Calculate for CLT
  clt_u[i] <- sqrt(n) * (mean_u[i] - 1/2)
}

# Plot the histogram of mean_u to demonstrate LLN
par(bg = 'white')
hist(mean_u, main = "LLN: Histogram of U-bar", xlab = "U-bar", ylab = "Frequency", col = "lightblue", border = "blue", breaks = 30)

# Plot the histogram of clt_u to demonstrate CLT
par(bg = 'white')
hist(clt_u, main = "CLT: Histogram of sqrt(n) (U-bar - 1/2)", xlab = "sqrt(n) (U-bar - 1/2)", ylab = "Frequency", col = "lightgreen", border = "darkgreen", breaks = 30)

