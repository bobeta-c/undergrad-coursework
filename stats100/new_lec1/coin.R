n <- 10

m <- 1000


coin_flips <- matrix(runif(n*m, min = 0, max = 1) < 0.5, nrow = m, ncol = n)


X <- rowSums(coin_flips)

par(mfrow = c(1, 2))
hist(X)
hist(X/n)
