#include<iostream>
#include<string>
#include<string.h>

using namespace std;


 typedef struct Boot
{
	char *name;
	int pg;
	int id;
}Boots;

void prininf1(Boots *book)
{
	cout << "name  : " << book->name <<endl;
	cout << "pg  : " << book->pg <<endl;
	cout << "id  : " << book->id <<endl;
}
void prininf(Boot book)
{
	cout << "name  : " << book.name <<endl;
	cout << "pg  : " << book.pg <<endl;
	cout << "id  : " << book.id <<endl;
}


class device
{
	public:
		static char *buf;
	
		virtual	void fun1() {cout << "device-fun1"<<endl;}

		device(int l,int w){len =l;a = w;}

		device(){};
		virtual ~device(){}; //析构函数加virtual 防止内存泄漏
		static void setdev(const int d) {len =d;}
		static int getdev() {return len;}

	private:
		int a;
		static int len;
		void fun2(){cout<<"dev-fun2"<<endl;}

	protected:
		int b;
		void fun3() {cout<<"dev-fun3"<<endl;}
		
	friend  void test(device &dev);// 一般不放在protect private public 中 仅仅声明并且要传类对象 做参数
};

class soc :public device
{
	public:
	   void fun1(){
		   fun2();
		   fun3();
		   cout << "soc-fun1"<<endl;
	   }

	private:
	    void fun2(){cout << "soc-fun2" <<endl;}

	protected: 
	   void fun3(){cout << "soc-fun3"<<endl;}
};


typedef struct CWFaceParam_t
{
    int roi_x, roi_y, roi_width, roi_height;    // 检测ROI, roi_x，roi_y为左上角坐标，roi_width为roi宽度，roi_height为roi高度，默认整帧图像
    int min_face_size, max_face_size;           // 人脸尺寸范围，默认[100,400],最大可设置范围[30,1500]，最大人脸尺寸不要超过ROI的roi_width和roi_height
    int max_face_num_perImg;                    // 单帧图像中最大可检测人脸数
    int perform_level;                          // 检测性能水平，范围(1~6).数字越大速度越慢，但效果（质量分和活体）越好,反之亦然.默认值5压缩至最大边长640
	int nir_face_compare;                       // 是否进行可见光和红外光人脸比对，目前仅红外活体使用，1:开启，0:关闭
    int open_quality;                           // 是否进行质量检测,1:开启，0:关闭，默认为true，设置为flase时以下float参数强制设置为-1不开启
    int open_liveness;                          // 是否进行活体,1:开启，0:关闭，默认关闭
	int face_align_in_detect;					// 是否在人脸检测回调中进行人脸对齐并返回对齐数据，1:开启，0:关闭，默认关闭
	int face_align_in_liveness;					// 是否在活体检测回调中进行人脸对齐并返回对齐数据，1:开启，0:关闭，默认关闭
    float pitch_min, pitch_max;                 // 点抬头阈值，pitch_min范围-90~0，默认-30度；   pitch_max范围0~90，默认30度，可根据实际使用需求调整
    float yaw_min, yaw_max;                     // 摇头阈值，  yaw_min范围-90~0，默认-30度；     yaw_max范围0~90，默认30度，可根据实际使用需求调整
    float roll_min, roll_max;                   // 摆头阈值，  roll_min范围-90~0，默认-30度；    roll_max范围0~90，默认30度，可根据实际使用需求调整
    float clarity;                              // 清晰度阈值，范围(0-1,-1)，默认0.3，设置为-1不开启，可根据实际使用需求调整
    float skinscore;                            // 肤色阈值，范围(0-1,-1)，默认0.35，设置为-1不开启，可根据实际使用需求调整
    float confidence;                           // 人脸置信度阈值，范围(0-1,-1)，默认0.55，设置为-1不开启，可根据实际使用需求调整
    float eyeopen;                              // 睁眼阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.5上下调整
    float mouthopen;                            // 张嘴阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.5上下调整
    float occlusion;                            // 遮挡阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.75上下调整
    float hight_brightness_threshold;           // 太亮阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.9上下调整
    float low_brightness_threshold;             // 太暗阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.2上下调整
    float blackspec;                            // 黑框眼镜阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.5上下调整
    float sunglass;                             // 墨镜阈值，范围(0-1,-1)，默认-1不开启，如非有相关需求使用默认值即可，开启后会增加耗时，如需设置建议在0.5上下调整
	float proceduremask;						// 是否检测口罩分数: 1检测，-1不检测
	float hat;									// 是否检测口罩分数: 1检测，-1不检测
	int gender;								// 是否检测性别分数: 1检测，-1不检测
	int age;									// 是否检测[年龄]: 1检测，-1不检测
	int emotion;								// 是否检测[表情]: 1检测，-1不检测
	int race;									// 是否检测[种族]: 1检测，-1不检测
}CWFaceParam;


extern"C" CWFaceParam __gCt = {
.hat = 2.8888,
.emotion = 777,
.face_align_in_detect = 66666,

};

void test(device &dev)
{
	cout << dev.b<<endl;
}

 int device::len; //使用前先定义 static

void ts(CWFaceParam_t *ct)
{
	cout<<ct->roi_y<<endl;
	cout<<ct->roi_x<<endl;
	cout<<ct->age<<endl;
}

int main()
{ 	string yq;

	char imei[10];
	int i=88;
	sprintf(imei,"imei%d",i);
	cout<<imei<<endl;
	
	if(strlen(yq.c_str()) != 0)
	{
		cout << "not null ls "<<yq.c_str()<<endl;
	}else{
		cout << "ls null"<<endl;
	}
	
	Boot boot1,boot2;
	boot1.name = (char*)"c++ str";
//	strcpy(boot1.name, "c++ str");
	boot1.pg = 66;
	boot1.id = 666;

	prininf(boot1);
	prininf1(&boot1);


	void *buf = malloc(sizeof(long));
	if(buf == NULL){
		
		cout<< "malloc failed "<<endl;
		return -1;

	}
	long b = 111111;

	memcpy(buf,&b,sizeof(long));

	cout<<"buf = "<<(long*)buf<<endl;
	long a;
	memset(&a,0,sizeof(long));
	memcpy(&a,buf,sizeof(void*));
	cout<< "mcpy ls "<<a<<endl;
	free(buf);
	buf = NULL;
///////////////////////////////
	device *dev = new soc();
	dev->fun1();
	cout << "----so -tst----"<<endl;
	soc *so =new soc();
	so->fun1();
//////////////////////////////
	 device::setdev(88888);// set 后再次set 也是88888
	int num = device::getdev();
      device::setdev(66666);
	cout <<num<<endl;
	int num2 = device::getdev();//依然还是 88888 
	cout <<num<<endl; 
/////////////////////////////
CWFaceParam pa ={1,2,3,4};
pa.race = 99900;
cout<<pa.race<<endl;
cout<<pa.roi_y<<endl;
	return 0;
/////////////////////////

ts(Cttt);






}
