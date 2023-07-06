import numpy as np

def print_results(path, print_prefix):
    acc = 0
    for i in range(10):
        result = np.fromfile(f'{path}/Result_{i}/class_probs.raw', dtype=np.float32)
        result = result.reshape(32,1000).argmax(axis=1)

        ground_truth = np.loadtxt(f'./ground_truth/labels_{i}.csv')
        acc += (result == ground_truth).sum()
    print(f'{print_prefix}: {acc / (10*32)}')

print_results('host_fp32', 'FP32 accuracy on host cpu')
print_results('cpu_fp32', 'FP32 accuracy on device cpu')
print_results('gpu_fp32', 'FP32 accuracy on device gpu')
print_results('htp_int8', 'INT8 accuracy on device htp') # slight decrease in accuracy