n <- 100
m <- 1000
coin_flips <- matrix(runif(n * m, min = 0, max = 1) < 0.5, nrow = m, ncol = n)
X <- rowSums(coin_flips)

pdf("fair.pdf")
par(mfrow = c(1,2))
hist(X, main = "X for a fair coin")
hist(X/n, main = "X/n for a fair coin")
dev.off()

