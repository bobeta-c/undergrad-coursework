n <- 100
m <- 1000
coin_flips <- matrix(runif(n * m, min = 0, max = 1) < 0.2, nrow = m, ncol = n)
X <- rowSums(coin_flips)
pdf("unfair.pdf")
par(mfrow = c(1,2))
hist(X, main="X for an unfair coin")
hist(X/n, main="X for an unfair coin")
dev.off()
