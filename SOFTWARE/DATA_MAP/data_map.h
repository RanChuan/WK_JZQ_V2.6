
#ifndef __DATA_MAP_H
#define __DATA_MAP_H



#ifdef __cplusplus
	extern "C" {
#endif





//保存屏幕配置的数据长度
#define LCD_DATA_NUM 20

//无线配置数据长度
#define CONFIG_DATA_NUM 200

//配置参数总长度
#define ALL_PARA_LENGTH (LCD_DATA_NUM+CONFIG_DATA_NUM)

//保存屏幕的配置
#define LCD_DATA getLcdCfgData()

//保存无线配置
#define EN_CONFIG  GetCfgData()



//我加的用于存储配置信息的函数
void Save_Config (void);
void Load_Config(void);

//保存屏幕的数据
void Save_LCD (void);
void Load_LCD(void);



enum {
	devTypeJzq=0,
	devTypeCjq=1,
	devTypeKt=2,
	devTypeCsj=3,
	devTypeJhq=4,
	devTypeJsj=5,
	devTypeYtj=6,
};





/****************************************************************
*******************具体操作函数
*****************************************************************/

#define DEVICE_DATA_NUM 20

#define EN_DATA GetCJDataAddr()

//找出设备数量
void Updata_DeviceNum (void);

//返回指定地址的设备类型
u8 GetDeviceType (u16 addr);

//返回指定地址的设备状态
u16 GetDeviceState(u16 addr);

//返回指定类型的设备数量
u16 GetDeviceNum (u8 type);

//返回第i个type类型的地址
u16 GetNextDeviceaddr (u8 type,u16 i);

//获取配置信息的首地址
u16 *GetCfgData (void);


//------------------------供外部调用的函数---------------------------


//获取采集器数据首地址
u8 *GetCJDataAddr (void);

//获取本机地址
u16 Get_MyAddr(void);

//设置本机地址
void Set_MyAddr(u16 addr);

//设置设备参数，格式是按照设备地址高8位，低8位，类型,len,设备个数
void Set_Device(u8 * data,u16 len);

//设置ip地址
void Set_MyIP (u8 *data);

//获取ip地址
u16 * Get_MyIP (void);

//设置本机无线信道
u8 Set_MyChanel (u8 chanel);

//获取本机无线信道
u8 Get_MyChanel (void);


//设置本地IP地址
u8 setNativeIP ( u8 * ip);

//设置本机端口
u8 setNativePort (u16 port );

//设置服务器ip地址
u8 setServerIP (u8 * ip);

//设置服务器端口
u8 setServerPort (u16 port );

//设置网关IP地址
u8 setGatewayIP (u8 *ip);

//添加设备0,成功，1，失败
u8 addDevAddr ( u8 dev,u16 addr );

//移除设备，0成功，1失败
u8 delDevAddr (u16 addr);

//清除所有配置的设备
void clearDev (void);


//------------------------供外部调用的函数End---------------------------

//根据类型获取名称
typedef struct 
{
	u8 objtype;
	u8 *name;
}objname;



//根据设备类型获取名称
u8 *dbg_getdevname(u8 devtype);


//----------------------供外部调用的函数-------------------

//获取是否手动的状态
u16 Lcd_GetHandstate (void);

//获取环境值的限度，0，温度上限，1，温度下限，2，湿度上限，3，湿度下限，4，tvoc上限
u16 Lcd_GetLimitData (u16 type);

//设置环境值限度
u16 Lcd_SetLimitData (u16 type,u16 newlimit);

//改变手动的状态
void Lcd_SetHandstate (u16 handstate);

//获取lcd屏幕的配置信息
u16 *getLcdCfgData (void);

//---------------------供外部调用的函数End-------------------





#ifdef __cplusplus
	}
#endif



#endif






