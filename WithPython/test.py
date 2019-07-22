from ctypes import cdll
sharedObj = cdll.LoadLibrary("./libfileCPP.so")
sharedObj.configureCamera()
returnVal = sharedObj.getCamData()
print(returnVal)
