import numpy as np

class Regression(object):
    def __init__(self, m=1, reg_param=0):
        """"
        Inputs:
          - m Polynomial degree
          - regularization parameter reg_param
        Goal:
         - Initialize the weight vector self.w
         - Initialize the polynomial degree self.m
         - Initialize the  regularization parameter self.reg
        """
        self.m = m
        self.reg  = reg_param
        self.dim = [m+1 , 1]
        self.w = np.zeros(self.dim)
        self.hist_grad = []
        self.hist_loss = []
        self.hist_preds = []
        self.hist_w = []
    def gen_poly_features(self, X):
        """
        Inputs:
         - X: A numpy array of shape (N,1) containing the data.
        Returns:
         - X_out an augmented training data to an mth degree feature vector e.g. [1, X, X^2, ..., X^m].
        """
        return X[:, 0:1] ** np.arange(self.m + 1)
            
    
    def loss_and_grad(self, X, y):
        """
        Inputs:
        - X: N x d array of training data.
        - y: N x 1 targets 
        Returns:
        - loss: a real number represents the loss 
        - grad: a vector of the same dimensions as self.w containing the gradient of the loss with respect to self.w 
        """
        m = self.m
        N,d = X.shape 
        y = y.reshape((N,1))
        assert d == 1
        X_h = self.gen_poly_features(X)
        preds = (X_h @ self.w).reshape(N,1)
        if self.reg == 0:
            loss = ((preds - y)**2).mean()
            grad = 2*(((preds-y)*X_h).mean(axis=0))
        else:
            loss = (0.5)*((preds - y)**2).sum() + self.reg * 0.5 * (self.w **2).sum()
            grad = ((preds-y)*X_h).sum(axis=0) + self.reg * (self.w).sum()

        # record history to find error
        # self.hist_preds.append(np.max(preds))
        # self.hist_grad.append(np.max(grad))
        # self.hist_w.append(np.max(self.w))
        # self.hist_loss.append(np.max(loss))

        return loss, grad
    
    def train_LR(self, X, y, eta=1e-3, batch_size=30, num_iters=1000) :
        """
        Finds the coefficients of a {d-1}^th degree polynomial
        that fits the data using least squares batch gradient descent.

        Inputs:
         - X         -- numpy array of shape (N,1), features
         - y         -- numpy array of shape (N,), targets
         - eta       -- float, learning rate
         - num_iters -- integer, maximum number of iterations
         
        Returns:
         - loss_history: vector containing the loss at each training iteration.
         - self.w: optimal weights 
        """
        loss_history = []
        N,d = X.shape
        for t in np.arange(num_iters):
                indices = np.random.choice(N,batch_size,replace=False)
                X_batch = X[indices]
                y_batch = y[indices]
                # ================================================================ #
                # YOUR CODE HERE:
                # Sample batch_size elements from the training data for use in gradient descent.  
                # After sampling, X_batch should have shape: (batch_size,1), y_batch should have shape: (batch_size,)
                # The indices should be randomly generated to reduce correlations in the dataset.  
                # Use np.random.choice.  It is better to user WITHOUT replacement.
                # ================================================================ #
                
                # ================================================================ #
                # END YOUR CODE HERE
                # ================================================================ #
                loss, grad = self.loss_and_grad(X_batch,y_batch)
                #if t < 10:
                    #print(loss, grad)
                self.w = self.w - eta * grad.reshape(-1,1)
                if not np.isfinite(loss) or not np.all(np.isfinite(grad)) or not np.all(np.isfinite(self.w)):
                    print(self.hist_grad[-20:])
                    print(self.hist_loss[-20:])
                    print(self.hist_preds[-20:])
                    print(self.hist_w[-20:])
                    raise ValueError(f"Training diverged at iteration {t}")
                # ================================================================ #
                # YOUR CODE HERE: 
                # evaluate loss and gradient for batch data
                # save loss as loss and gradient as grad
                # update the weights self.w
                # ================================================================ #
                
                # ================================================================ #
                # END YOUR CODE HERE
                # ================================================================ #
                loss_history.append(loss)
        return loss_history, self.w
    def closed_form(self, X, y):
        """
        Inputs:
        - X: N x 1 array of training data.
        - y: N x 1 array of targets
        Returns:
        - self.w: optimal weights 
        """
        N,d = X.shape
        assert d == 1
        X_h = self.gen_poly_features(X)
        N, d = X_h.shape
        self.w = np.linalg.solve(X_h.T @ X_h + self.reg * np.identity(d), X_h.T @ y)
        return self.loss_and_grad(X,y)
    
    
    def predict(self, X):
        """
        Inputs:
        - X: N x 1 array of training data.
        Returns:
        - y_pred: Predicted targets for the data in X. y_pred is a 1-dimensional
          array of length N.
        """
        m = self.m
        X = self.gen_poly_features(X)
        return  X @ self.w
        
# import matplotlib.pyplot as plt   
# a = Regression(m=6)
# X = np.arange(10).reshape(10,1)
# y = np.sin(X)
# history, w = a.train_LR(X,y,eta = 1e-7,batch_size=10, num_iters= 10000)
# plt.plot(X,a.predict(X))
# plt.plot(X,y, color = 'red')
# plt.show()
# print(history[-1])