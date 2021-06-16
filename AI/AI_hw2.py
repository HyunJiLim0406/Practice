import numpy as np
import h5py
from random import randint

# load MNIST dataset
MNIST_data = h5py.File('MNISTdata.hdf5', 'r')
x_train = np.float32(MNIST_data['x_train'][:])
y_train = np.int32(np.array(MNIST_data['y_train'][:, 0]))
x_test = np.float32(MNIST_data['x_test'][:])
y_test = np.int32(np.array(MNIST_data['y_test'][:, 0]))
MNIST_data.close()


class CONV_LAYER:
    def __init__(self, dim_ifmap, num_inch, dim_kernel, num_outch, padding):
        self.dim_ifmap = dim_ifmap
        self.num_inch = num_inch
        self.dim_kernel = dim_kernel
        self.num_outch = num_outch
        self.padding = padding

        self.kernels = np.random.rand(num_inch, num_outch, dim_kernel, dim_kernel) / np.sqrt(
            num_inch * num_outch * dim_kernel * dim_kernel)

    def forward(self, ifmap):
        self.dim_ofmap = (self.dim_ifmap - self.dim_kernel + 2 * self.padding) + 1
        padded_ifmap = np.pad(ifmap, ((0, 0), (self.padding, self.padding), (self.padding, self.padding)), 'constant')
        ofmap = np.zeros((self.num_outch, self.dim_ofmap, self.dim_ofmap), dtype=float)
        for x in range(self.dim_ofmap):
            for y in range(self.dim_ofmap):
                for k in range(self.num_outch):
                    for c in range(self.num_inch):
                        for i in range(self.dim_kernel):
                            for j in range(self.dim_kernel):
                                ofmap[k, x, y] += self.kernels[c, k, i, j] * padded_ifmap[c, x + i, y + j]
        return ofmap

    def backprop(self, I, dO):

        padded_I = np.pad(I, ((0, 0), (self.padding, self.padding), (self.padding, self.padding)), 'constant')
        dK = np.zeros((self.num_inch, self.num_outch, self.dim_kernel, self.dim_kernel), dtype=float)
        for x in range(self.dim_ofmap):
            for y in range(self.dim_ofmap):
                for k in range(self.num_outch):
                    for c in range(self.num_inch):
                        for i in range(self.dim_kernel):
                            for j in range(self.dim_kernel):
                                dK[c, k, i, j] += padded_I[c, x + i, y + j] * dO[k, x, y]
        # your job: dI

        return dK


class FC_LAYER:

    def __init__(self, num_in, num_out):
        self.kernel = np.random.randn(num_in, num_out) / np.sqrt(num_in * num_out)
        self.bias = np.random.randn(1, num_out) / np.sqrt(num_out)

    def forward(self, x):
        z = np.dot(x, self.kernel) + self.bias
        return z

    def backprop(self, x, dZ2):
        dW = np.dot(x.T, dZ2)
        dZ1 = np.dot(dZ2, self.kernel.T)
        dB = np.sum(dZ2, axis=0, keepdims=True)
        return dW, dZ1, dB


class RELU_LAYER:
    def forward(self, x):
        return x * (x > 0)

    def backprop(self, x):
        return 1.0 * (x > 0)


def softmax(x):
    return np.exp(x) / np.sum(np.exp(x))


class CROSS_ENTROPY_ERROR:
    def forward(self, x, y):
        return -1.0 * np.sum(np.multiply(np.log(x + 0.001e-10), y))

    def backprop(self, x, y):
        return (x - y)


conv1 = CONV_LAYER(dim_ifmap=28, num_inch=1, dim_kernel=3, num_outch=5, padding=1)
relu1 = RELU_LAYER()
fc1 = FC_LAYER(28 * 28 * 5, 10)
cse1 = CROSS_ENTROPY_ERROR()
lr = 0.001
num_epochs = 3
test_iterations = 100
batch_size = 2
train_iterations = int(1000 / batch_size)

for epoch in range(num_epochs):

    total_trained = 0
    train_correct = 0
    train_cost = 0
    rand_indices = np.random.choice(len(x_train), (train_iterations, batch_size), replace=True)
    #mini_batch = int(1000 / batch_size)
    for i in rand_indices:
        batch_dK_CONV1 = 0
        batch_dW_FC1 = 0
        batch_dB_FC1 = 0
        total_trained += 1
        for j in i:
            # 미니 배치 수정
            #batch_mask = np.random.choice(rand_indices, batch_size)
            mini_x_train = x_train[j].reshape(1, 28, 28)
            mini_y_train = y_train[j]
            one_hot_y = np.zeros((1, 10), dtype=float)
            one_hot_y[np.arange(1), mini_y_train] = 1.0

            # forward propagation
            conv1_ofmap = conv1.forward(mini_x_train)
            relu1_ofmap = relu1.forward(conv1_ofmap)
            fc1_out = fc1.forward(relu1_ofmap.reshape(1, 28 * 28 * 5))
            prob = softmax(fc1_out)
            train_cost += cse1.forward(prob, one_hot_y)

            # backpropagation
            dCSE1 = cse1.backprop(prob, one_hot_y)
            dW_FC1, dZ_FC1, dB_FC1 = fc1.backprop(relu1_ofmap.reshape(1, 28 * 28 * 5), dCSE1)
            dRELU1 = relu1.backprop(conv1_ofmap)
            dK_CONV1 = conv1.backprop(mini_x_train, np.multiply(dRELU1, dZ_FC1.reshape(5, 28, 28)))
            train_correct += np.sum(np.equal(np.argmax(prob, axis=1), mini_y_train))

        # weight update
        conv1.kernels -= lr * (batch_dK_CONV1 / batch_size)
        fc1.kernel -= lr * (batch_dW_FC1 / batch_size)
        fc1.bias -= lr * (batch_dB_FC1 / batch_size)


        if (total_trained % 100 == 0):
            print("Trained: ", total_trained, "/", train_iterations, "\ttrain accuracy: ", train_correct / 100,
                  "\ttrain cost: ", train_cost / 100)
            train_cost = 0
            train_correct = 0

    test_correct = 0
    for i in range(test_iterations):
        mini_x_test = x_test[i].reshape(1, 28, 28)
        mini_y_test = y_test[i]

        conv1_ofmap = conv1.forward(mini_x_test)
        relu1_ofmap = relu1.forward(conv1_ofmap)
        fc1_out = fc1.forward(relu1_ofmap.reshape(1, 28 * 28 * 5))
        prob = softmax(fc1_out)
        test_correct += np.sum(np.equal(np.argmax(prob, axis=1), mini_y_test))
    print("epoch # ", epoch, "\ttest accuracy: ", test_correct / test_iterations)
