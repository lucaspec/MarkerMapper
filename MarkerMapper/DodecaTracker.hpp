//
//  DodecaTracker.hpp
//  MarkerMapper
//
//  Created by 郭子乐 on 2018/4/5.
//  Copyright © 2018年 郭子乐. All rights reserved.
//

#ifndef DodecaTracker_hpp
#define DodecaTracker_hpp

#include "stdafx.h"

#include "Pen.hpp"
#include "Camera.hpp"
#include "PenDetector.hpp"

using namespace std;

// 相机模式 （实时视频或是读取文件）
enum Camera_Mode {VIDEO_NONE, VIDEO_LIVE, VIDEO_FILE};

// 管理相机和笔和追踪
class DodecaTracker{
private:
    // 相机参数
    string camera_parameter_path;
    int video_capture_index;
    string video_file_path;
    Camera_Mode camera_mode;
    
    cv::Mat camera_pose;
    // 笔参数
    string marker_map_path;
    
    Camera camera;
    Pen pen;

    PenDetector pen_detector;
    
    bool getCamera(){
        aruco::MarkerDetector md;
        md.setDictionary(dictionary);
        md.getParameters().setCornerRefinementMethod(aruco::CornerRefinementMethod::CORNER_LINES);
        
        camera.setCameraParameter(camera_parameter_path);
        if(camera_mode==VIDEO_LIVE) {
            camera.setVideoCapture(video_capture_index);
            // 注：限定相机！！！！！！！！！！
            camera.setVideoCaptureParameters1080p();
        }
        else if(camera_mode==VIDEO_FILE) camera.setVideoCapture(video_file_path);
        else return false;
        camera.setCameraPose(camera_pose);
        camera.setMarkerDetector(md);
        
        return true;
    }
    bool getPen(){
        pen.setMarkerMap(marker_map_path);
        
        return true;
    }
    
public:
    //---------------- 参数 -------------------------
    // marker 参数
    // 相机标定变量
    static float calibration_marker_size;    //标定用marker的边长
    // 正十二面体变量
    static aruco::Dictionary::DICT_TYPES dictionary;  //正十二面体表面marker所属的字典
    static float dodeca_marker_size;       //正十二面体表面marker边长
    
    DodecaTracker(){
        camera_mode = VIDEO_NONE;
    }
    
    // --------- 初始化 --------
    bool initCamera(string camera_parameter_path, int video_capture_index=0, Mat camera_pose = cv::Mat::eye(4, 4, CV_32F)){
        fstream file;
        file.open(camera_parameter_path);
        if(!file.is_open()) return false;
        
        this->camera_parameter_path = camera_parameter_path;
        this->video_capture_index = video_capture_index;
        this->camera_mode = VIDEO_LIVE;
        this->camera_pose = camera_pose;
        
        getCamera();
        
        return true;
    }
    bool initCamera(string camera_parameter_path, string video_file_path, Mat camera_pose = cv::Mat::eye(4, 4, CV_32F)){
        fstream file;
        file.open(camera_parameter_path);
        if(!file.is_open()) return false;
//        fstream video_file;
//        video_file.open(video_file_path);
//        if(!video_file.is_open()) return false;
        
        this->camera_parameter_path = camera_parameter_path;
        this->video_file_path = video_file_path;
        this->camera_mode = VIDEO_FILE;
        this->camera_pose = camera_pose;
        
        getCamera();
        
        return true;
    }
    bool initPen(string marker_map_path){
        fstream file;
        file.open(marker_map_path);
        if(!file.is_open()) return false;
        
        this->marker_map_path = marker_map_path;
        
        getPen();
        
        return true;
    }
    bool setPenTip(const string& pentip_path){
        fstream file;
        file.open(pentip_path);
        if(!file.is_open()) return false;
        
        pen.setPenTip(pentip_path);
        return true;
    }
    bool setDodecaCenter(const string& center_path){
        fstream file;
        file.open(center_path);
        if(!file.is_open()) return false;
        
        pen.setDodecaCenter(center_path);
        return true;
    }
    bool setPenTip(const Mat& rt_mat){
        pen.pentip_pose = rt_mat.clone();
        return true;
    }
    bool setDodecaCenter(const Mat& rt_mat){
        pen.dodeca_center_pose = rt_mat.clone();
        return true;
    }
    bool initPenDetector(){
        if(!(camera.isValid()&&pen.isValid())) return false;
        
        pen_detector = PenDetector(&camera, &pen);
        
        return pen_detector.isValid();
    }
    
    //------- 相机位置标定 ---------
    int detectMarkers(){
        int frame_index = camera.retrieve();
        return camera.detectMarkers(false, false);
    }
    bool calibrateCameraPose(string calib_marker_map_path){
        return camera.calibratePose(calib_marker_map_path);
    }
    cv::Mat getCameraPose(){
        assert(!camera.camera_pose.empty());
        Mat pose;
        camera.camera_pose.copyTo(pose);
        return pose;
    }
    
    //---------- 笔姿态检测 ---------
    // 等待下一帧
    bool grab(){
        return pen_detector.grabOneFrame();
    }
    
    // 检测当前帧中笔的位置
    bool detect(){
        return pen_detector.detectOneFrame();
    }
    
    // 获得当前帧指定笔的世界坐标的pose（4*4 float）
    cv::Mat getPose(){
        return pen.getFrame(camera.next_frame_index-1);
    }
    
    // 获得笔尖的姿态（4*1 float）
    bool getPenTipPose(cv::Mat& mat){
        if(pen.pentip_pose.empty()) {
            cout << "PEN::ERROR::No pentip position." << endl;
            return false;
        }else{
            mat = pen.pentip_pose.clone();
            return true;
        }
    }
    // 获得正十二面体中心的姿态（4*1 float）
    bool getDodecaCenterPose(cv::Mat& mat){
        if(pen.dodeca_center_pose.empty()) {
            cout << "PEN::ERROR::No pentip position." << endl;
            return false;
        }else{
            mat =  pen.dodeca_center_pose.clone();
            return true;
        }
    }
    
    bool isValid(){
        return pen_detector.isValid();
    }
    
    bool reset(){
        if(!pen_detector.isValid()) return false;
        return camera.reset();
    }
};

#endif /* DodecaTracker_hpp */
