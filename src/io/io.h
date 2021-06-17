#if defined (STM32F4XX)
#  define GPIO_RESET_BIT(port,pin)       do { (port)->BSRRH = (pin); } while (0)
#  define GPIO_SET_BIT(port,pin)         do { (port)->BSRRL = (pin); } while (0)
#elif defined (STM32F10X)
#  define GPIO_RESET_BIT(port,pin)       do { (port)->BRR  = (pin); } while (0)
#  define GPIO_SET_BIT(port,pin)         do { (port)->BSRR = (pin); } while (0)
#endif
