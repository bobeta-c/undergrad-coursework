n <- 1000
x <- runif(n, min=-1, max = 1)
y <- runif(n, min=-1, max = 1)
points_inside <- sum(x^2+y^2 < 1)
print((4*points_inside)/n)
