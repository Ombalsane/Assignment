import psutil

running_pids = []
for process in psutil.process_iter(['pid']):
    running_pids.append(process.info['pid'])

print("List of running PIDs:", running_pids)
