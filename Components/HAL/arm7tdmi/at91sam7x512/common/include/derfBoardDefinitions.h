#ifndef _DERF_BOARD_DEFINITIONS_H_
#define _DERF_BOARD_DEFINITIONS_H_

#define PIN_SET_AS_PERIPHERAL_B(pin, pio) do {                          \
    AT91S_PIO *pio_ptr=NULL;                                            \
    if(pio == PIO_A)                                                    \
        pio_ptr = AT91C_BASE_PIOA;                                      \
    else if(pio == PIO_B)                                               \
        pio_ptr = AT91C_BASE_PIOB;                                      \
                                                                        \
    if(pio_ptr != NULL) {                                               \
        /* Bit corresponding to pin is set in B select register. */     \
        pio_ptr->PIO_BSR = pin;                                         \
        /* PIO control is disabled on the specified pin. */             \
        pio_ptr->PIO_PDR = pin;                                         \
    }                                                                   \
} while (0);

#define PIN_SET_AS_PIO_OUTPUT(pin, pio) do {                            \
    AT91S_PIO *pio_ptr=NULL;                                            \
    if(pio == PIO_A)                                                    \
        pio_ptr = AT91C_BASE_PIOA;                                      \
    else if(pio == PIO_B)                                               \
        pio_ptr = AT91C_BASE_PIOB;                                      \
    if(NULL != pio_ptr) {                                               \
        /* Set Bit corresponding to pin in output enable register. */   \
        pio_ptr->PIO_OER = pin;                                         \
        /* PIO control is enabled on the specified pin. */              \
        pio_ptr->PIO_PER = pin;                                         \
    }                                                                   \
} while (0);

#define _BV(x) (1 << (x))

/*
 * Identifiers for PIO's in AT91SAM7X512
 */
typedef enum pio_type_tag
{
	PIO_A,
	PIO_B
} pio_type_t;

/* RESET pin */
#define RST                             (AT91C_PIO_PA9)

/* Sleep Transceiver pin */
#define SLP_TR                          (AT91C_PIO_PA8)

/* Slave select pin */
#define SEL                             (AT91C_PIO_PA21)

/* SPI Bus Master Input/Slave Output pin */
#define MISO                            (AT91C_PIO_PA24)

/* SPI Bus Master Output/Slave Input pin */
#define MOSI                            (AT91C_PIO_PA23)

/* SPI serial clock pin */
#define SCK                             (AT91C_PIO_PA22)

#define PERIPHERAL_CLOCK_ENABLE(id)     (AT91C_BASE_PMC->PMC_PCER = _BV(id))
#endif
