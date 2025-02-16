t <- 1000000

U <- runif(t, min =0, max =1)

Z <- ifelse(U <0.5, -1, 1)

X <- rep(0, t+1)

for(i in 1:t) {
    X[i +1] <- X[i] + Z[i]
}

#
plot(0:t, X)

