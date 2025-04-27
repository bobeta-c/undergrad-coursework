n <- 1000

U <- runif(n, min = 0, max = 1)

X <- -log(U)

hist(X)
