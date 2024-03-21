#include "digitron.h"

/****************数码管段码表：共阳**********************/
const unsigned char Tab[] = {0xc0, 0xf9, 0xa4, 0xb0,              // 0~3
                             0x99, 0x92, 0x82, 0xf8,              // 4~7
                             0x80, 0x90, 0x88, 0x83,              // 8~9  A b
                             0xc6, 0xa1, 0x86, 0x8e, 0xff, 0x7f}; // C d E F 灭 .

/****************数码管段码表：共阴**********************/
// const unsigned char Tab[] = {0x3f, 0x06, 0x5b, 0x4f,			  // 0~3
// 							 0x66, 0x6d, 0x7d, 0x07,			  // 4~7
// 							 0x7f, 0x6F, 0x77, 0x7c,			  // 8~9  A b
// 							 0x39, 0x5e, 0x79, 0x71, 0x00, 0x80}; // C d E F 灭 .

spi_device_handle_t spi_handle;

#define LOAD_PIN 23
#define CLK_PIN 22
#define SDI_PIN 21

esp_err_t spi_write(spi_device_handle_t spi, uint8_t *data, uint8_t len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0)
        return -1;            // no need to send anything
    memset(&t, 0, sizeof(t)); // Zero out the transaction

    t.length = len * 8;                         // Len is in bytes, transaction length is in bits.
    t.tx_buffer = data;                         // Data
    t.user = (void *)1;                         // D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    assert(ret == ESP_OK);                      // Should have had no issues.

    return ret;
}

void digitron_clear(spi_device_handle_t spi)
{
    uint8_t clear[] = {0xff};
    for (int i = 0; i < 16; i++)
        spi_write(spi, clear, 1);
}

void digitron_display_error()
{
    spi_device_handle_t spi = spi_handle;
    digitron_clear(spi);
    spi_write(spi, &Tab[16], 1);
    spi_write(spi, &Tab[10], 1);
    spi_write(spi, &Tab[10], 1);
    spi_write(spi, &Tab[0], 1);
    spi_write(spi, &Tab[10], 1);
}
void digitron_display_int(int num)
{
    spi_device_handle_t spi = spi_handle;
    digitron_clear(spi);
    int d1, d2, d3, d4; // d1,d2,d3,d4 分别代表千位、百位、十位、个位
    d4 = num % 10;
    d3 = (num / 10) % 10;
    d2 = (num / 100) % 10;
    d1 = num / 1000;
    spi_write(spi, &Tab[d4], 1);
    spi_write(spi, &Tab[d3], 1);
    spi_write(spi, &Tab[d2], 1);
    spi_write(spi, &Tab[d1], 1);
}
void digitron_init(void)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,         // 不设置
        .mosi_io_num = SDI_PIN,    // 主机输出
        .sclk_io_num = CLK_PIN,    // 时钟信号
        .quadwp_io_num = -1,       // WP信号线，专用于QSPI的D2
        .quadhd_io_num = -1,       // HD信号线，专用于QSPI的D3
        .max_transfer_sz = 64 * 8, // 最大传输数据大小
    };                             // 总线配置结构体
    esp_err_t e = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    if (e != ESP_OK)
        ESP_LOGI("SPI_BUS", "bus initialize failed!");
    else
        ESP_LOGI("SPI_BUS", "bus initialize successed!");
    spi_device_interface_config_t device_config = {
        .clock_speed_hz = SPI_MASTER_FREQ_10M, // Clock out at 10 MHz,
        .mode = 0,                             // SPI mode 0
        .spics_io_num = -1,
        .queue_size = 7, // 传输队列大小，决定了等待传输数据的数量

    };
    // device_config.post_cb = ;
    e = spi_bus_add_device(SPI2_HOST, &device_config, &spi_handle);
    if (e != ESP_OK)
        ESP_LOGI("SPI_BUS", "device config error");
    else
        ESP_LOGI("SPI_BUS", "device config successed");
    spi_transaction_t transaction_config;                       // 定义数据结构体
    memset(&transaction_config, 0, sizeof(transaction_config)); // Zero out the transaction
    u_int8_t data[] = {0xf9};
    transaction_config.tx_buffer = data;
    transaction_config.length = 8;
    transaction_config.user = (void *)1; // D/C needs to be set to 1
    digitron_clear(spi_handle);
}
