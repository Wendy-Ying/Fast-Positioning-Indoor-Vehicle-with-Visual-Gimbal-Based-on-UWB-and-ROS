from machine import Timer, PWM
import sensor, time, image, lcd, gc
from fpioa_manager import fm
from maix import GPIO
from machine import UART


# 初始化传感器
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 设置像素格式为 RGB565
sensor.set_framesize(sensor.QVGA)    # 设置帧大小为 QVGA (320x240)
sensor.set_vflip(True)               # 垂直翻转
sensor.set_hmirror(True)             # 水平镜像
sensor.set_auto_gain(False, gain_db=0.0)
sensor.set_auto_whitebal(True)
sensor.skip_frames(time=2000)        # 跳过前2000ms的帧
lcd.init()                           # 初始化 LCD

# 初始化定时器和 PWM 用于舵机控制
tim1 = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PWM)
tim2 = Timer(Timer.TIMER0, Timer.CHANNEL1, mode=Timer.MODE_PWM)
pwm_x = PWM(tim1, freq=50, duty=7.5, pin=11, enable=True)  # 水平方向舵机连接到引脚 11
pwm_y = PWM(tim2, freq=50, duty=7.5, pin=12, enable=True)  # 垂直方向舵机连接到引脚 12

# 屏幕中心位置
center_x = 160
center_y = 120

# PID 参数
Kp = 0.2
Ki = 0.01
Kd = 0.1

# PID 变量
previous_error_x = 0
integral_x = 0
previous_error_y = 0
integral_y = 0

# 最大调整值
max_adjustment = 0.05

# 红色斑点的颜色阈值 (根据需要调整)
red_threshold = (80, 100, -15, 31, 0, 17)

while True:
    img = sensor.snapshot()  # 获取一帧图像
    blobs = img.find_blobs([red_threshold], x_stride=2, y_stride=1, area_threshold=10, pixels_threshold=10, merge=True, margin=10)

    if blobs:
        largest_blob = max(blobs, key=lambda b: b.pixels())  # 找到最大的斑点
        img.draw_rectangle(largest_blob.rect())              # 在图像上绘制矩形框
        img.draw_cross(largest_blob.cx(), largest_blob.cy()) # 在斑点中心绘制十字

        # 水平 PID 控制
        error_x = largest_blob.cx() - center_x
        integral_x += error_x
        derivative_x = error_x - previous_error_x
        output_x = Kp * error_x + Ki * integral_x + Kd * derivative_x

        # 垂直 PID 控制
        error_y = largest_blob.cy() - center_y
        integral_y += error_y
        derivative_y = error_y - previous_error_y
        output_y = Kp * error_y + Ki * integral_y + Kd * derivative_y

        # 控制水平方向舵机
        current_duty_x = pwm_x.duty()
        adjustment_x = min(max(output_x, -max_adjustment), max_adjustment)
        new_duty_x = current_duty_x - adjustment_x
        new_duty_x = max(min(new_duty_x, 12.5), 2.5)
        pwm_x.duty(new_duty_x)

        # 控制垂直方向舵机
        current_duty_y = pwm_y.duty()
        adjustment_y = min(max(output_y, -max_adjustment), max_adjustment)
        new_duty_y = current_duty_y - adjustment_y
        new_duty_y = max(min(new_duty_y, 12.5), 2.5)
        pwm_y.duty(new_duty_y)

        # 更新前一误差
        previous_error_x = error_x
        previous_error_y = error_y

    lcd.display(img)  # 显示图像
    time.sleep_ms(10)
    gc.collect()
