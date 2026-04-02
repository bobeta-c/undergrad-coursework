# %%
import torch as t
import csv
import numpy as np
import random
import torch.utils.data
import pandas as pd
# %%

header = ['age', 'work', 'fnlwgt', 'edu', 'yredu', 'marriage', 'occupation',
 'relationship', 'race', 'sex', 'capgain', 'caploss', 'workhr', 'country']
df = pd.read_csv(
    "adult.data",
    names=header,
    index_col=False)

# %%
for x in ["age", "yredu", "capgain", "caploss", "workhr"]:
    print(x, df[x].min(),df[x].max(),df[x].mean())
    df[x] = (df[x] - df[x].mean())/(df[x].std())

# %%

contcols = ["age", "yredu", "capgain", "caploss", "workhr"]
catcols = ["work", "marriage", "occupation", "edu", "relationship", "sex"]
features = contcols + catcols
df = df[features]

# %%
missing = pd.concat([df[c] == " ?" for c in catcols], axis=1).any(axis=1)
df_with_missing = df[missing]
df_not_missing = df[~missing]

# %%
set(df_not_missing["work"])
# %%
data = pd.get_dummies(df_not_missing)
# %%
datanp = data.values.astype(np.float32)
# %%
cat_index = {}  # Mapping of feature -> start index of feature in a record
cat_values = {} # Mapping of feature -> list of categorical values the feature can take

# build up the cat_index and cat_values dictionary
for i, header in enumerate(data.keys()):
    if "_" in header: # categorical header
        feature, value = header.split()
        feature = feature[:-1] # remove the last char; it is always an underscore
        if feature not in cat_index:
            cat_index[feature] = i
            cat_values[feature] = [value]
        else:
            cat_values[feature].append(value)
# %%
def get_onehot(record,feature):
    return get_range(record, feature)
def get_range(record, feature):
    """
    Return the portion of `record` that is the one-hot encoding
    of `feature`. For example, since the feature "work" is stored
    in the indices [5:12] in each record, calling `get_range(record, "work")`
    is equivalent to accessing `record[5:12]`.
    
    Args:
        - record: a numpy array representing one record, formatted
                  the same way as a row in `data.np`
        - feature: a string, should be an element of `catcols`
    """
    start_index = cat_index[feature]
    stop_index = cat_index[feature] + len(cat_values[feature])
    return record[start_index:stop_index]

def get_categorical_value(onehot, feature):
    """
    Return the categorical value name of a feature given
    a one-hot vector representing the feature.
    
    Args:
        - onehot: a numpy array one-hot representation of the feature
        - feature: a string, should be an element of `catcols`
        
    Examples:
    
    >>> get_categorical_value(np.array([0., 0., 0., 0., 0., 1., 0.]), "work")
    'State-gov'
    >>> get_categorical_value(np.array([0.1, 0., 1.1, 0.2, 0., 1., 0.]), "work")
    'Private'
    """
    index = onehot.argmax()
    return cat_values[feature][index]

# %%
def get_feature(record, feature):
    """
    Return the categorical feature value of a record
    """
    onehot = get_onehot(record, feature)
    return get_categorical_value(onehot, feature)

def get_features(record):
    """
    Return a dictionary of all categorical feature values of a record
    """
    return { f: get_feature(record, f) for f in catcols }
# %%
# %%
import torch as t

# set the numpy seed for reproducibility
# https://docs.scipy.org/doc/numpy/reference/generated/numpy.random.seed.html
np.random.seed(50)
np.random.shuffle(datanp)
train_list, val_list, test_list =datanp[:int(len(datanp)*0.70)], datanp[int(len(datanp)*0.7):int(len(datanp)*0.85)], datanp[int(len(datanp)*0.85):]
print(train_list.shape[0],val_list.shape[0],test_list.shape[0])
# %%
datanp.shape

# %%
from torch import nn

class AutoEncoder(nn.Module):
    def __init__(self, compression_size = 8):
        super(AutoEncoder, self).__init__()
        self.compression_size = compression_size
        self.encoder = nn.Sequential(
            nn.Linear(57, 32), # TODO -- FILL OUT THE CODE HERE!
            nn.ReLU(),
            nn.Linear(32,16),
            nn.ReLU(),
            nn.Linear(16,self.compression_size)
        )
        self.decoder = nn.Sequential(
            nn.Linear(self.compression_size, 16), # TODO -- FILL OUT THE CODE HERE!
            nn.ReLU(),
            nn.Linear(16,32),
            nn.ReLU(),
            nn.Linear(32,57),
            nn.Sigmoid() # get to the range (0, 1)
        )

    def forward(self, x):
        x = self.encoder(x)
        x = self.decoder(x)
        return x
# %%


def zero_out_feature(records, feature):
    """ Set the feature missing in records, by setting the appropriate
    columns of records to 0
    """
    start_index = cat_index[feature]
    stop_index = cat_index[feature] + len(cat_values[feature])
    records[start_index:stop_index] = 0
    return records

def zero_out_random_feature(records):
    """ Set one random feature missing in records, by setting the 
    appropriate columns of records to 0
    """
    return zero_out_feature(records, random.choice(catcols))

def get_accuracy(input_row, output_row):
    input_features = get_features(input_row)
    output_features = get_features(output_row)
    errors = 0
    total = len(input_features.keys())
    for key in input_features.keys():
        if input_features[key] != output_features[key]:
            errors += 1
    return 1 - errors/total

def evaluate(net,loader,criterion):
    total_loss = 0.0
    total_acc = 0.0
    total_epoch = 0
    for i, data in enumerate(loader, 0):
        data = t.from_numpy(data)
        datam = zero_out_random_feature(data.clone())
        recon = net(datam)
        loss = criterion(recon, data)
        total_loss += loss.item()
        total_acc += get_accuracy(data,recon)
        total_epoch += 1
    acc = float(total_acc) / total_epoch
    loss = float(total_loss) / (i + 1)
    return acc, loss
def get_model_name(model,learning_rate,epoch):
    return "AutoEncoder_compression_size{}_lr{}_epoch{}".format(model.compression_size,learning_rate,epoch)
def plot_training_curve(path):
    """ Plots the training curve for a model run, given the csv files
    containing the train/validation error/loss.

    Args:
        path: The base path of the csv files produced during training
    """
    import matplotlib.pyplot as plt
    train_err = np.loadtxt("{}_train_acc.csv".format(path))
    val_err = np.loadtxt("{}_val_acc.csv".format(path))
    train_loss = np.loadtxt("{}_train_loss.csv".format(path))
    val_loss = np.loadtxt("{}_val_loss.csv".format(path))
    plt.title("Train vs Validation accuracy")
    n = len(train_err) # number of epochs
    plt.plot(range(1,n+1), train_err, label="Train")
    plt.plot(range(1,n+1), val_err, label="Validation")
    plt.xlabel("Epoch")
    plt.ylabel("Error")
    plt.legend(loc='best')
    plt.show()
    plt.title("Train vs Validation Loss")
    plt.plot(range(1,n+1), train_loss, label="Train")
    plt.plot(range(1,n+1), val_loss, label="Validation")
    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.legend(loc='best')
    plt.show()
import time
def train(model, train_loader, valid_loader, num_epochs=5, learning_rate=1e-4):
    """ Training loop. You should update this."""
    torch.manual_seed(42)
    criterion = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
    trainLoss, valLoss = np.zeros(num_epochs), np.zeros(num_epochs)
    trainAcc, valAcc = np.zeros(num_epochs), np.zeros(num_epochs)
    start_time = time.time()
    for epoch in range(num_epochs):
        total_train_loss = 0.0
        total_train_acc = 0.0
        total_epoch = 0
        for data in train_loader:
            data = t.from_numpy(data)
            datam = zero_out_random_feature(data.clone()) # zero out one categorical feature
            recon = model(datam)
            loss = criterion(recon, data)
            loss.backward()
            optimizer.step()
            optimizer.zero_grad()
            total_train_loss += loss.item()
            total_train_acc += get_accuracy(data,recon)
            total_epoch += 1
        trainAcc[epoch] = float(total_train_acc) / total_epoch
        trainLoss[epoch] = float(total_train_loss) / total_epoch
        valAcc[epoch], valLoss[epoch] = evaluate(model, valid_loader,criterion)
        print(("Epoch {}: Train acc: {}, Train loss: {} |"+
               "Validation acc: {}, Validation loss: {}").format(
                   epoch + 1,
                   trainAcc[epoch],
                   trainLoss[epoch],
                   valAcc[epoch],
                   valLoss[epoch]))
        model_path = get_model_name(model,learning_rate,epoch)
        t.save(model.state_dict(),model_path)
    print("finished training")
    end_time = time.time()
    elapsed_time = end_time - start_time
    print("total time elapsed: {:.2f} seconds".format(elapsed_time))
    np.savetxt("{}_train_acc.csv".format(model_path), trainAcc)
    np.savetxt("{}_train_loss.csv".format(model_path),trainLoss)
    np.savetxt("{}_val_acc.csv".format(model_path),valAcc)
    np.savetxt("{}_val_loss.csv".format(model_path), valLoss)
# %%
net = AutoEncoder()
train(net, num_epochs=20, train_loader= train_list, valid_loader=val_list)


# %%
type(datanp)
# %%
plot_training_curve(get_model_name(net, learning_rate = 1e-4, epoch=19))
# %%
net2 = AutoEncoder(9)
train(net2, num_epochs=100, train_loader=train_list, valid_loader= val_list)
# %%
plot_training_curve(get_model_name(net2,learning_rate= 1e-4, epoch=39))

# %%
def get_accuracy(model, data_loader):
    """Return the "accuracy" of the autoencoder model across a data set.
    That is, for each record and for each categorical feature, 
    we determine whether the model can successfully predict the value
    of the categorical feature given all the other features of the 
    record. The returned "accuracy" measure is the percentage of times 
    that our model is successful.
        
    Args:
       - model: the autoencoder model, an instance of nn.Module
       - data_loader: an instance of torch.utils.data.DataLoader

    Example (to illustrate how get_accuracy is intended to be called.
             Depending on your variable naming this code might require
             modification.)

        >>> model = AutoEncoder()
        >>> vdl = torch.utils.data.DataLoader(data_valid, batch_size=256, shuffle=True)
        >>> get_accuracy(model, vdl)
    """
    total = 0
    acc = 0
    for col in catcols:
        for item in data_loader: # minibatches
            item = t.from_numpy(item)
            inp = item.numpy()
            out = model(zero_out_feature(item.clone(), col)).detach().numpy()
            acc += int(get_feature(out, col) == get_feature(inp, col))
            total += 1
    return acc / total
# %%
get_accuracy(net2, test_list)
# %%
