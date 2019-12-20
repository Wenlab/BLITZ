import h2o
import numpy as np
import pandas as pd

def predict_left(List):
    PyList=List
    print(PyList)
    df=pd.DataFrame(np.zeros((1,40)),columns=range(1,41))
    df.iloc[0]=PyList
    test_predict=h2o.mojo_predict_pandas(df, 'E:/autoML/left_true_0513_accuracy_09516/GBM_grid_1_AutoML_20190513_170806_model_7.zip')
    print(test_predict)
    return int(test_predict.iat[0,0])

def predict_right(List):
    PyList=List
    print(PyList)
    df=pd.DataFrame(np.zeros((1,40)),columns=range(1,41))
    df.iloc[0]=PyList
    test_predict=h2o.mojo_predict_pandas(df, 'E:/autoML/right_true_accuracy_09516/GBM_5_AutoML_20190502_170602.zip')
    print(test_predict)
    return int(test_predict.iat[0,0])
