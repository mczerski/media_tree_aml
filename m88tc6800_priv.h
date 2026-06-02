#ifndef __M88TC6800_PRIV_H__
#define __M88TC6800_PRIV_H__

#include "m88tc6800.h"

typedef unsigned char    byte;
typedef unsigned long long    ulonglong;
typedef unsigned short    undefined2;

struct m88tc6800_priv {
	struct m88tc6800_config config;
	struct i2c_adapter *i2c;
	bool mcu_status;
	u32 frequency_hz;
	u32 bandwidth_hz;
	u32 int_ldiv;
	u32 fvco_tg;
};

int m88tc6800_write_reg(struct m88tc6800_priv *priv, u8 reg, u8 val);
int m88tc6800_write_data(struct m88tc6800_priv *priv, u8 *data, size_t len);
int m88tc6800_read_reg(struct m88tc6800_priv *priv, u8 reg, u8 *val);
int m88tc6800_set_reg_bits(struct m88tc6800_priv *priv, u8 reg, u8 val, u8 begin_bit, u8 end_bit);
void m88tc6800_preset(struct m88tc6800_priv *priv);
void m88tc6800_set_rf_frontend(struct m88tc6800_priv *priv, u32 freq_khz);
u8 m88tc6800_get_mixer_type(struct m88tc6800_priv *priv, u32 freq_khz);
void m88tc6800_set_mixer(struct m88tc6800_priv *priv, u32 freq_khz, u8 mixer_type);
void m88tc6800_set_lo(struct m88tc6800_priv *priv, u32 freq_khz, u8 mixer_type);
void m88tc6800_set_pll(struct m88tc6800_priv *priv);
void m88tc6800_set_bandwidth(struct m88tc6800_priv *priv, u32 bandwidth_hz);
void m88tc6800_set_dac(struct m88tc6800_priv *priv, u32 freq_khz, u32 bandwidth_hz);
int m88tc6800_set_freq(struct m88tc6800_priv *priv, u32 freq_khz, u32 bandwidth_hz);
int m88tc6800_get_rf_power(struct m88tc6800_priv *priv, s32 *strength);

#endif /* __M88TC6800__PRIV_H__ */
