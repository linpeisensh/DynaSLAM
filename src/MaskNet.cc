/**
* This file is part of DynaSLAM.
*
* Copyright (C) 2018 Berta Bescos <bbescos at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/bertabescos/DynaSLAM>.
*
*/

#include "MaskNet.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <dirent.h>
#include <errno.h>

namespace DynaSLAM
{

#define U_SEGSt(a)\
    gettimeofday(&tvsv,0);\
    a = tvsv.tv_sec + tvsv.tv_usec/1000000.0
struct timeval tvsv;
double t1sv, t2sv,t0sv,t3sv;
void tic_initsv(){U_SEGSt(t0sv);}
void toc_finalsv(double &time){U_SEGSt(t3sv); time =  (t3sv- t0sv)/1;}
void ticsv(){U_SEGSt(t1sv);}
void tocsv(){U_SEGSt(t2sv);}
// std::cout << (t2sv - t1sv)/1 << std::endl;}

SegmentDynObject::SegmentDynObject(){
    std::cout << "Importing Mask R-CNN Settings..." << std::endl;
    ImportSettings();
//    std::string x;
//    setenv("PYTHONPATH", this->py_path.c_str(), 1);
//    x = getenv("PYTHONPATH");
//    std::cout << x << std::endl;
    Py_Initialize();
    PyRun_SimpleString("print('Python Start')");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./src/python/')");
    PyRun_SimpleString("from maskrcnn_benchmark.config import cfg");
    PyRun_SimpleString("from demo.predictor import COCODemo");
    this->cvt = new NDArrayConverter();
    this->py_module = PyImport_ImportModule("MaskRCNN");
    std::cout << "hello world!" << std::endl;
    assert(this->py_module != NULL);
    std::cout << "hello world!" << std::endl;
    PyObject *pDict = PyModule_GetDict(this->py_module);
    std::cout << "0" << std::endl;
//    this->py_class = PyDict_GetItemString(pyDict, this->class_name.c_str());
//    this->py_class = PyObject_GetAttrString(this->py_module, this->class_name.c_str());
    this->py_class = PyDict_GetItemString(pDict, "Mask");
    std::cout << "1!" << std::endl;
    assert(this->py_class != NULL);
    std::cout << "2" << std::endl;
    this->net = PyInstance_New(this->py_class, NULL, NULL);
//    this->net = PyInstanceMethod_New(this->py_class);
    std::cout << "3" << std::endl;
    assert(this->net != NULL);
    std::cout << "Creating net instance..." << std::endl;
    cv::Mat image  = cv::Mat::zeros(480,640,CV_8UC3); //Be careful with size!!
    std::cout << "Loading net parameters..." << std::endl;
    GetSegmentation(image);
}

SegmentDynObject::~SegmentDynObject(){
    delete this->py_module;
    delete this->py_class;
    delete this->net;
    delete this->cvt;
}

cv::Mat SegmentDynObject::GetSegmentation(cv::Mat &image,std::string dir, std::string name){
    cv::Mat seg = cv::imread(dir+"/"+name,CV_LOAD_IMAGE_UNCHANGED);
    if(seg.empty()){
        PyObject* py_image = cvt->toNDArray(image.clone());
        assert(py_image != NULL);
        PyObject* py_mask_image = PyObject_CallMethod(this->net, const_cast<char*>(this->get_dyn_seg.c_str()),"(O)",py_image);
        seg = cvt->toMat(py_mask_image).clone();
        seg.cv::Mat::convertTo(seg,CV_8U);//0 background y 1 foreground
        if(dir.compare("no_save")!=0){
            DIR* _dir = opendir(dir.c_str());
            if (_dir) {closedir(_dir);}
            else if (ENOENT == errno)
            {
                const int check = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (check == -1) {
                    std::string str = dir;
                    str.replace(str.end() - 6, str.end(), "");
                    mkdir(str.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                }
            }
            cv::imwrite(dir+"/"+name,seg);
        }
    }
    return seg;
}

void SegmentDynObject::ImportSettings(){
    std::string strSettingsFile = "./Examples/RGB-D/MaskSettings.yaml";
    cv::FileStorage fs(strSettingsFile.c_str(), cv::FileStorage::READ);
    fs["py_path"] >> this->py_path;
    fs["module_name"] >> this->module_name;
    fs["class_name"] >> this->class_name;
    fs["get_dyn_seg"] >> this->get_dyn_seg;

    // std::cout << "    py_path: "<< this->py_path << std::endl;
    // std::cout << "    module_name: "<< this->module_name << std::endl;
    // std::cout << "    class_name: "<< this->class_name << std::endl;
    // std::cout << "    get_dyn_seg: "<< this->get_dyn_seg << std::endl;
}


}






















