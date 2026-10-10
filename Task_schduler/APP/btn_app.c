#include "btn_app.h"
#include "ebtn.h"

/* 1. 定义按键参数实例 */
// 参数宏: EBTN_PARAMS_INIT(
//     按下消抖时间ms, 释放消抖时间ms,
//     单击有效最短按下时间ms, 单击有效最长按下时间ms,
//     多次单击最大间隔时间ms,
//     长按(KeepAlive)事件周期ms (0禁用),
//     最大连续有效点击次数 (e.g., 1=单击, 2=双击, ...)
// )

const ebtn_btn_param_t defaul_ebtn_param = EBTN_PARAMS_INIT(
    20,     // time_debounce: 按下稳定 20ms
    20,     // time_debounce_release: 释放稳定 20ms
    50,     // time_click_pressed_min: 最短单击按下 50ms
    500,    // time_click_pressed_max: 最长单击按下 500ms (超过则不算单击)
    300,    // time_click_multi_max: 多次单击最大间隔 300ms (两次点击间隔超过则重新计数)
    500,    // time_keepalive_period: 长按事件周期 500ms (按下超过 500ms 后，每 500ms 触发一次)
    1       // max_consecutive: 只识别单击，释放后立即上报
);


/* 2. 定义静态按键列表 */
// 宏: EBTN_BUTTON_INIT(按键ID, 参数指针)
//ebtn_btn_t static_buttons[] = {
//    EBTN_BUTTON_INIT(1, &key_param_normal), // KEY1, ID=1, 使用 'key_param_normal' 参数
//    EBTN_BUTTON_INIT(2, &key_param_normal), // KEY2, ID=2, 也使用 'key_param_normal' 参数
//		EBTN_BUTTON_INIT(3, &key_param_normal), // KEY1, ID=1, 使用 'key_param_normal' 参数
//    EBTN_BUTTON_INIT(4, &key_param_normal), // KEY2, ID=2, 也使用 'key_param_normal' 参数
//		EBTN_BUTTON_INIT(5, &key_param_normal), // KEY1, ID=1, 使用 'key_param_normal' 参数
//    EBTN_BUTTON_INIT(6, &key_param_normal), // KEY2, ID=2, 也使用 'key_param_normal' 参数
//};
typedef enum
{
    USER_BUTTON_0 = 0, // 这里定义 0，后面会自动递增
    USER_BUTTON_1,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_4,
    USER_BUTTON_5,
    USER_BUTTON_MAX,
	USER_BUTTON_01=7,
	USER_BUTTON_02=8,
} user_button_t;

// 绑定按键 ID 和默认参数
static ebtn_btn_t btns[] = 
{
	EBTN_BUTTON_INIT(USER_BUTTON_1, &defaul_ebtn_param), // 默认参数
	EBTN_BUTTON_INIT(USER_BUTTON_2, &defaul_ebtn_param), // 枚举名比直接写 1 更直观
	EBTN_BUTTON_INIT(USER_BUTTON_3, &defaul_ebtn_param),
	EBTN_BUTTON_INIT(USER_BUTTON_4, &defaul_ebtn_param),
	EBTN_BUTTON_INIT(USER_BUTTON_5, &defaul_ebtn_param),
	EBTN_BUTTON_INIT(USER_BUTTON_MAX, &defaul_ebtn_param)
};
//静态组合按键数组
static ebtn_btn_combo_t btn_combine[]={
	EBTN_BUTTON_COMBO_INIT(USER_BUTTON_01, &defaul_ebtn_param), // 默认参数
	EBTN_BUTTON_COMBO_INIT(USER_BUTTON_02, &defaul_ebtn_param) 
};
/* 1. 实现获取按键状态的回调函数 */
// 函数原型: uint8_t (*ebtn_get_state_fn)(struct ebtn_btn *btn);
uint8_t my_get_key_state(struct ebtn_btn *btn) {
    // 根据传入的按钮实例中的 key_id 判断是哪个物理按键
    switch (btn->key_id) {
        case 1: // 请求读取 KEY1 的状态
            return (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET);
        case 2: // 请求读取 KEY2 的状态
            return (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET);
        // ... 可以添加更多按键的读取逻辑 ...
		case 3:
			return (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET);
		case 4:
			return (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET);
		case 5:
			return (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET);
	    case 6:
			return (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == GPIO_PIN_RESET);
        default:
            // 对于库内部处理组合键等情况，或者未知的 key_id，安全起见返回 0 (未按下)
            return 0;
    }
    // 注意：返回值 1 表示 "活动/按下"，0 表示 "非活动/释放"
}

/* 2. 实现处理按键事件的回调函数 */
// 函数原型: void (*ebtn_evt_fn)(struct ebtn_btn *btn, ebtn_evt_t evt);
//void my_handle_key_event(struct ebtn_btn *btn, ebtn_evt_t evt) 
//{
//    uint16_t key_id = btn->key_id;                 // 获取触发事件的按键 ID
//    uint16_t click_cnt = ebtn_click_get_count(btn); // 获取连击次数 (仅在 ONCLICK 事件时有意义)
//    // uint16_t kalive_cnt = ebtn_keepalive_get_count(btn); // 获取长按计数 (仅在 KEEPALIVE 事件时有意义)

//   //调试打印 (可选)
////    printf("Key ID: %d, Event: %d", key_id, evt);

//    // 根据事件类型进行处理
//    switch (evt) {
//        case EBTN_EVT_ONPRESS: // 按下事件 (消抖成功后触发一次)
//            // printf(" - Pressed\n");
//            // 可以在这里处理按下即触发的操作，比如点亮提示灯
//            if (key_id == 1) { /* Do something for KEY1 press */ }
//            break;

//        case EBTN_EVT_ONRELEASE: // 释放事件 (消抖成功后触发一次)
//            // printf(" - Released\n");
//            // 可以在这里处理释放时触发的操作
//             if (key_id == 1) { /* Do something for KEY1 release */ }
//            break;

//        case EBTN_EVT_ONCLICK: // 单击/连击事件 (在释放后，或达到最大连击数，或超时后触发)
//            // printf(" - Clicked (%d times)\n", click_cnt);
//            // --- 根据 key_id 和 click_cnt 执行不同操作 ---
//            if (key_id == 3) { // 如果是 KEY3 触发的 CLICK
//                if (click_cnt == 1) {
//                    // KEY3 单击
////                     my_printf(&huart1,"  Action: KEY1 Single Click - Toggle LED1\n");
//										 ucled[2]^=1;	
//                    // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//                } else if (click_cnt == 2) {
//                    // KEY2 双击
////                     my_printf(&huart1,"  Action: KEY1 Double Click - Switch Mode\n");
//										 ucled[2]=0;
//                }
//                // ... 可以继续判断 3击, 4击 ...
//            } else if (key_id == 2) { // 如果是 KEY2 触发的 CLICK
//                 if (click_cnt == 1) {
//                    // KEY2 单击
//                    // printf("  Action: KEY2 Single Click - Toggle LED2\n");
//                 }
//            } else if (key_id == 7) { // 如果是组合键 (KEY1+KEY2) 触发的 CLICK
//                if (click_cnt == 1) {
//                     // 组合键单击
//                     // printf("  Action: Combo Key 101 Single Click - Reset System\n");
//                }
//            }
//            break;

//        case EBTN_EVT_KEEPALIVE: // 保持活动/长按事件 (按下持续时间超过阈值后，按周期触发)
//            // printf(" - Keep Alive (Long Press, Count: %d)\n", kalive_cnt);
//            if (key_id == 1) {
//                // KEY1 长按
//                // printf("  Action: KEY1 Long Press - Increase Volume\n");
//            }
//            break;

//        default: // 未知事件 (理论上不应发生)
//            // printf(" - Unknown Event\n");
//            break;
//    }
//			
//    
//}
void prv_btn_event(struct ebtn_btn *btn, ebtn_evt_t evt)
{
//    if (evt == EBTN_EVT_KEEPALIVE) // 长按事件
//    {
//        switch (btn->key_id)
//        {
//        case USER_BUTTON_1:
//            WOUOUI_MSG_QUE_SEND(msg_up);
//            break;
//        case USER_BUTTON_2:
//            WOUOUI_MSG_QUE_SEND(msg_down);
//            break;
//        case USER_BUTTON_3:
//            WOUOUI_MSG_QUE_SEND(msg_left);
//            break;
//        case USER_BUTTON_4:
//            WOUOUI_MSG_QUE_SEND(msg_right);
//            break;
//        case USER_BUTTON_5:
//            WOUOUI_MSG_QUE_SEND(msg_return);
//            break;
//        case USER_BUTTON_MAX:
//            WOUOUI_MSG_QUE_SEND(msg_click);
//            break;
//        default:
//            break;
//        }
//    }
//    else
//    {
				if(evt==EBTN_EVT_ONCLICK)
				{
        if ((btn->key_id == USER_BUTTON_1) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_up);
        }

        if ((btn->key_id == USER_BUTTON_2) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_down);
        }

        if ((btn->key_id == USER_BUTTON_3) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_left);
        }

        if ((btn->key_id == USER_BUTTON_4) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_right);
        }

        if ((btn->key_id == USER_BUTTON_5) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_return); // 返回
        }

        if ((btn->key_id == USER_BUTTON_MAX) && (ebtn_click_get_count(btn) == 1))
        {
            WOUOUI_MSG_QUE_SEND(msg_click); // 确定
        }
			}
//    }

    //    if(btn->key_id == USER_BUTTON_COMBO_0) {
    //        rlog_str("btn combo click evt");
    //    }
}
void app_ebtn_init(void)
{
    ebtn_init(btns, EBTN_ARRAY_SIZE(btns), btn_combine, 2, my_get_key_state, prv_btn_event);//传入空指针，0
		/*绑定组合按键*/
	  //    ebtn_combo_btn_add_btn(&btns_combo[0], USER_BUTTON_0);
    //    ebtn_combo_btn_add_btn(&btns_combo[0], USER_BUTTON_1);

    //    ebtn_combo_btn_add_btn(&btns_combo[1], USER_BUTTON_2);
    //    ebtn_combo_btn_add_btn(&btns_combo[1], USER_BUTTON_3);
}

//
void btn_task(void)
{
	ebtn_process(HAL_GetTick());
}





