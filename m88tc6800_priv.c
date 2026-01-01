#include "m88tc6800_priv.h"

int m88tc6800_set_reg_bits(struct m88tc6800_priv *priv, u8 reg, u8 val, u8 begin_bit, u8 end_bit)
{
	u8 val_mask = ((1 << (end_bit - begin_bit + 1)) - 1) << begin_bit;
	u8 reg_data;
	int ret;

	ret = m88tc6800_read_reg(priv, reg, &reg_data);
	if (ret)
		return ret;

	reg_data &= ~val_mask;
	val = (val << begin_bit) & val_mask;
	reg_data |= val;

  ret = m88tc6800_write_reg(priv, reg, reg_data);
  return ret;
}

u32 m88tc6800_expand_reg_bits(struct m88tc6800_priv *priv, u8 reg_expand, u8 reg, u8 val, u8 start_bit, u8 end_bit)
{
  byte reg_data_00;
  uint8_t reg_addr_00 = reg_expand;
  uint8_t reg_data_01 = reg;
  int extraout_r2 = val;
  uint extraout_r3 = end_bit;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  byte local_31 [5];
  
  uVar2 = (uint)start_bit;
  local_31[0] = 0;
  uVar1 = uVar2;
  uVar3 = extraout_r3;
  if (uVar2 <= extraout_r3) {
    uVar1 = extraout_r3;
    uVar3 = uVar2;
  }
  uVar3 = (uVar3 + 7) - uVar1;
  m88tc6800_write_reg(priv, reg_addr_00,reg_data_01);
  m88tc6800_read_reg(priv, reg_addr_00 + 0x1,local_31);
  reg_data_00 = (local_31[0] & ~(byte)((int)(0xff << (uVar3 & 0xff) & 0xffU) >> (7 - uVar1 & 0xff))) |
                (byte)((int)(extraout_r2 << (uVar3 & 0xff) & 0xffU) >> (7 - uVar1 & 0xff));
  local_31[0] = reg_data_00;
  m88tc6800_write_reg(priv, reg_addr_00,reg_data_01);
  m88tc6800_write_reg(priv, reg_addr_00 + 0x1,reg_data_00);
  return 0;
}

void m88tc6800_preset(struct m88tc6800_priv *priv)
{
    u8 reg_data;

    if (0 < priv->config.mode && priv->config.mode <= 2) {
        m88tc6800_write_reg(priv, 0x0b, 0x7d);
    }
    else {
        m88tc6800_write_reg(priv, 0x0b, 0x6d);
    }
    m88tc6800_write_reg(priv, 0x0c, 0xe3);
    m88tc6800_write_reg(priv, 0x0d, 0xc4);
    m88tc6800_read_reg(priv, 0x9a, &reg_data);
    m88tc6800_write_reg(priv, 0x9a, reg_data & 0xf);
    m88tc6800_write_reg(priv, 0x9d, 0x3f);
    m88tc6800_write_reg(priv, 0x31, 0x97);
    m88tc6800_write_reg(priv, 0xab, 0xd9);
    m88tc6800_write_reg(priv, 0xb0, 0x09);
    m88tc6800_write_reg(priv, 0xbd, 0x02);
    m88tc6800_write_reg(priv, 0xb9, 0x99);
    m88tc6800_write_reg(priv, 0x58, 0x11);
    if (priv->config.xtal == 24000) {
        m88tc6800_write_reg(priv, 0x39, 0x77);
        m88tc6800_write_reg(priv, 0x3a, 0x11);
        m88tc6800_write_reg(priv, 0x7e, 0x12);
    }
    else if (priv->config.xtal == 27000) {
        m88tc6800_write_reg(priv, 0x7e, 0x13);
    }
    m88tc6800_write_reg(priv, 0x80, 0x00);
    m88tc6800_write_reg(priv, 0x82, 0x00);
    m88tc6800_write_reg(priv, 0x10, 0x00);
    m88tc6800_write_reg(priv, 0x66, 0x00);
}

void m88tc6800_set_rf_frontend(struct m88tc6800_priv *priv, u32 freq_khz)
{
    u8 reg_data;

    if (priv->config.int_im == 0) {
        m88tc6800_write_reg(priv, 0x88, 0x27);
        m88tc6800_write_reg(priv, 0x89, 0x2b);
        m88tc6800_write_reg(priv, 0x8a, 0x67);
        m88tc6800_write_reg(priv, 0x39, 0x69);
        m88tc6800_write_reg(priv, 0x3a, 0x12);
        if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
            m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x22);
            m88tc6800_write_reg(priv, 0xfd, 0x1a);
        }
        else {
            m88tc6800_write_reg(priv, 0x8b, 0xaa);
            m88tc6800_write_reg(priv, 0xfd, 0x12);
        }
    }
    else if (priv->config.int_im == 1) {
        m88tc6800_write_reg(priv, 0x88, 0x1b);
        m88tc6800_write_reg(priv, 0x89, 0x2b);
        m88tc6800_write_reg(priv, 0x8a, 0x1b);
        m88tc6800_write_reg(priv, 0x39, 0x69);
        m88tc6800_write_reg(priv, 0x3a, 0x12);
        if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
            m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x22);
            m88tc6800_write_reg(priv, 0xfd, 0x1a);
        }
        else {
            m88tc6800_write_reg(priv, 0x8b, 0xaa);
            m88tc6800_write_reg(priv, 0xfd, 0x12);
        }
    }
    else if (priv->config.int_im == 2) {
        m88tc6800_write_reg(priv, 0x88, 0x2b);
        m88tc6800_write_reg(priv, 0x89, 0x2b);
        m88tc6800_write_reg(priv, 0x8a, 0xab);
        m88tc6800_write_reg(priv, 0x39, 0x69);
        if (freq_khz <= 602000) {
            m88tc6800_write_reg(priv, 0x3a, 0x2d);
            if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x55);
                m88tc6800_write_reg(priv, 0xfd, 0x1a);
            }
            else {
                m88tc6800_read_reg(priv, 0x8b, &reg_data);
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : reg_data | 0x88);
                m88tc6800_write_reg(priv, 0xfd, 0x12);
            }
        }
        else {
            m88tc6800_write_reg(priv, 0x3a, 0x09);
            if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x11);
                m88tc6800_write_reg(priv, 0xfd, 0x1a);
            }
            else {
                m88tc6800_read_reg(priv, 0x8b, &reg_data);
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : reg_data | 0x88);
                m88tc6800_write_reg(priv, 0xfd, 0x12);
            }
        }
    }
    else {
        m88tc6800_write_reg(priv, 0x88, 0x2b);
        m88tc6800_write_reg(priv, 0x89, 0x2b);
        m88tc6800_write_reg(priv, 0x8a, 0xab);
        m88tc6800_write_reg(priv, 0x39, 0x69);
        if (freq_khz <= 602000) {
            m88tc6800_write_reg(priv, 0x3a, 0x2d);
            if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x55);
                m88tc6800_write_reg(priv, 0xfd, 0x1a);
            }
            else {
                m88tc6800_write_reg(priv, 0x8b, 0xaa);
                m88tc6800_write_reg(priv, 0xfd, 0x12);
            }
        }
        else {
            m88tc6800_write_reg(priv, 0x3a, 0x09);
            if ((priv->config.mode == 0 || priv->config.mode > 2) || priv->config.int_lt < 2) {
                m88tc6800_write_reg(priv, 0x8b, priv->config.custom_cfg == 1 ? 0xaa : 0x11);
                m88tc6800_write_reg(priv, 0xfd, 0x1a);
            }
            else {
                m88tc6800_write_reg(priv, 0x8b, 0xaa);
                m88tc6800_write_reg(priv, 0xfd, 0x12);
            }
        }
    }
    if (freq_khz > 112000) {
        if (freq_khz <= 334000) {
            m88tc6800_write_reg(priv, 0x7f, 0x04);
            m88tc6800_write_reg(priv, 0x92, 0x8c);
            m88tc6800_write_reg(priv, 0x87, 0xf0);
            m88tc6800_write_reg(priv, 0x90, 0x49);
        }
        else {
            m88tc6800_write_reg(priv, 0x7f, 0x00);
            m88tc6800_write_reg(priv, 0x92, 0x8c);
            m88tc6800_write_reg(priv, 0x87, 0xf0);
            if (freq_khz <= 451000) {
                m88tc6800_write_reg(priv, 0x90, 0x49);
            }
            else {
                m88tc6800_write_reg(priv, 0x90, 0x4a);
            }
        }
    }
    else {
        m88tc6800_write_reg(priv, 0x7f, 0x08);
        m88tc6800_write_reg(priv, 0x92, 0x8c);
        m88tc6800_write_reg(priv, 0x87, 0xf0);
        m88tc6800_write_reg(priv, 0x90, 0x49);
    }
    m88tc6800_write_reg(priv, 0xaa, 0x84);
    m88tc6800_write_reg(priv, 0xa9, 0xf6);
    m88tc6800_write_reg(priv, 0x98, 0xf8);
    if (freq_khz > 700000 && freq_khz <= 900000) {
        m88tc6800_write_reg(priv, 0x99, 0xa5);
    }
    else {
        m88tc6800_write_reg(priv, 0x99, 0xb6);
    }
    m88tc6800_write_reg(priv, 0x26, 0x11);
    if (priv->config.harmonic_imp != 1) {
        return;
    }
    if (freq_khz >= 366000 && freq_khz <= 374000) {
        m88tc6800_write_reg(priv, 0x94, 0x29);
    }
    else if ((freq_khz >= 480000 && freq_khz <= 492000) || (freq_khz >= 610000 && freq_khz <= 624000)) {
        m88tc6800_write_reg(priv, 0x94, 0x15);
    }
    else {
        m88tc6800_write_reg(priv, 0x94, 0x01);
    }
    m88tc6800_write_reg(priv, 0x39, 0x0e);
    m88tc6800_write_reg(priv, 0x3a, 0x04);
    m88tc6800_write_reg(priv, 0x39, 0x0f);
    m88tc6800_write_reg(priv, 0x3a, 0x04);
    m88tc6800_write_reg(priv, 0x39, 0x10);

    if ((freq_khz >= 366000 && freq_khz <= 374000) ||
            (freq_khz >= 480000 && freq_khz <= 492000) ||
            (freq_khz >= 610000 && freq_khz <= 624000) ||
            (freq_khz >= 800000 && freq_khz <= 820000)) {
        m88tc6800_write_reg(priv, 0x3a, 0x02);
    }
    else {
        m88tc6800_write_reg(priv, 0x3a, 0x01);
    }
}

u8 m88tc6800_get_mixer_type(struct m88tc6800_priv *priv, u32 freq_khz)
{
    if (freq_khz <= 334000) {
        return 2;
    }
    if (priv->config.harmonic_imp == 1) {
        if ((freq_khz >= 480000 && freq_khz <= 492000) ||
               (freq_khz >= 610000 && freq_khz <= 624000) ||
               (freq_khz >= 800000 && freq_khz <= 820000) ||
               (freq_khz >= 366000 && freq_khz <= 374000)) {
            return 3;
        }
        return 1;
    }
    else {
        return 1;
    }
}

void m88tc6800_set_mixer(struct m88tc6800_priv *priv, u32 freq_khz, u8 mixer_type)
{
    u8 tmp = mixer_type;
    if (mixer_type == 2) {
        if (freq_khz >= 125000) {
            if (freq_khz < 167000) {
                m88tc6800_write_reg(priv, 0x2f, 0x10);
            }
            else if (freq_khz < 250000) {
                m88tc6800_write_reg(priv, 0x2f, 0x21);
            }
            else if (freq_khz < 335000) {
                m88tc6800_write_reg(priv, 0x2f, 0x32);
            }
            else {
                m88tc6800_write_reg(priv, 0x2f, 0x43);
            }
        }
        else {
            m88tc6800_write_reg(priv, 0x2f, 0x00);
        }
    }
    else {
        if (mixer_type != 3) {
            tmp = 1;
        }
        m88tc6800_write_reg(priv, 0x2f, 0x00);
    }
    m88tc6800_set_reg_bits(priv, 0x0b, tmp, 0, 1);
    if (freq_khz >= 330000 && freq_khz <= 400000) {
        m88tc6800_write_reg(priv, 0x32, 0x04);
    }
    else {
        m88tc6800_write_reg(priv, 0x32, 0x00);
    }
}

void m88tc6800_set_lo(struct m88tc6800_priv *priv, u32 freq_khz, u8 mixer_type)
{
    u8 reg_data;
    if (freq_khz < 500000 && mixer_type == 2) {
        if (freq_khz < 63000) {
            reg_data = 0x8b;
            priv->ldiv = 0x80;
        }
        else if (freq_khz < 83000) {
            reg_data = 0x7a;
            priv->ldiv = 0x60;
        }
        else if (freq_khz < 125000) {
            reg_data = 0x0a;
            priv->ldiv = 0x40;
        }
        else if (freq_khz < 167000) {
            reg_data = 0x0d;
            priv->ldiv = 0x30;
        }
        else if (freq_khz < 250000) {
            reg_data = 0x09;
            priv->ldiv = 0x20;
        }
        else if (freq_khz < 335000) {
            reg_data = 0x0c;
            priv->ldiv = 0x18;
        }
        else {
            reg_data = 0x08;
            priv->ldiv = 0x10;
        }
    }
    else if (freq_khz < 250000 || mixer_type != 3) {
        if (freq_khz < 56000) {
            reg_data = 0x0b;
            priv->ldiv = 0x80;
        }
        else if (freq_khz < 83000) {
            reg_data = 0x7a;
            priv->ldiv = 0x60;
        }
        else if (freq_khz < 125000) {
            reg_data = 0x0a;
            priv->ldiv = 0x40;
        }
        else if (freq_khz < 167000) {
            reg_data = 0x79;
            priv->ldiv = 0x30;
        }
        else if (freq_khz < 250000) {
            reg_data = 0x09;
            priv->ldiv = 0x20;
        }
        else if (freq_khz < 335000) {
            reg_data = 0x78;
            priv->ldiv = 0x18;
        }
        else if (freq_khz < 500000) {
            reg_data = 0x08;
            priv->ldiv = 0x10;
        }
        else if (freq_khz < 667000) {
            reg_data = 0x74;
            priv->ldiv = 0x0c;
        }
        else {
            reg_data = 0x04;
            priv->ldiv = 0x08;
        }
    }
    else {
        if (freq_khz < 333000) {
            reg_data = 0xf8;
            priv->ldiv = 0x18;
        }
        else if (freq_khz < 500000) {
            reg_data = 0x88;
            priv->ldiv = 0x10;
        }
        else if (freq_khz < 666000) {
            reg_data = 0x74;
            priv->ldiv = 0x0c;
        }
        else {
            reg_data = 0x04;
            priv->ldiv = 0x08;
        }
    }
    priv->fvco_tg = freq_khz * priv->ldiv;
    if (priv->fvco_tg < 6700000) {
        m88tc6800_set_reg_bits(priv, 0x13, 4, 3, 5);
    }
    else {
        m88tc6800_set_reg_bits(priv, 0x13, 7, 3, 5);
    }
    m88tc6800_write_reg(priv, 0x14, reg_data);
}

void m88tc6800_set_pll(struct m88tc6800_priv *priv)
{
  uint uVar1;
  byte bVar2;
  uint32_t uVar3;
  uint32_t uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint8_t uVar8;
  int extraout_r1;
  int extraout_r1_00;
  char cVar9;
  int iVar10;
  byte bVar11;
  uint uVar12;
  uint uVar13;
  uint8_t reg_data;
  uint uVar14;
  uint uVar15;
  bool bVar16;
  uint uVar17;
  byte local_38;

  uVar4 = priv->fvco_tg;
  uVar3 = priv->config.xtal;
  uVar1 = priv->config.xtal >> 1;
  uVar12 = priv->config.xtal * 2;
  if (priv->config.xtal == 24000) {
    iVar10 = priv->fvco_tg / priv->ldiv;
    if (0x176e < iVar10 - 0x518b1U) {
      cVar9 = '\0';
      uVar13 = 48000;
      goto LAB_00089804;
    }
    uVar6 = 0x5dc;
    cVar9 = '\0';
    uVar13 = 24000;
LAB_00089808:
    uVar5 = uVar4 / uVar6;
    extraout_r1_00 = uVar4 % uVar6;
    uVar6 = (extraout_r1_00 << 0xc) / uVar6;
  }
  else {
    if (priv->config.xtal == 27000) {
      iVar10 = (priv->fvco_tg / 27000 -
               (((int)((ulonglong)(priv->fvco_tg >> 2) * 0x4dac1b9d >>
                      0x20) << 0x15) >> 0x1f)) * 27000;
      if (((iVar10 + 10U < priv->fvco_tg) &&
          (priv->fvco_tg < iVar10 + 0x3f2U)) ||
         ((priv->fvco_tg < iVar10 - 10U &&
          (iVar10 - 0x3f2U < priv->fvco_tg)))) {
        cVar9 = '\x02';
        uVar13 = 18000;
      }
      else if ((iVar10 + 0x3fcU < priv->fvco_tg) &&
              (priv->fvco_tg < iVar10 + 0x7e4U)) {
        cVar9 = '\x01';
        uVar13 = 18000;
      }
      else if (priv->fvco_tg < iVar10 - 0x3fcU) {
        cVar9 = iVar10 - 0x7e4U < priv->fvco_tg;
        uVar13 = 18000;
        if (!(bool)cVar9) {
          uVar13 = 54000;
        }
      }
      else {
        cVar9 = '\0';
        uVar13 = 54000;
      }
LAB_00089804:
      uVar6 = uVar13 >> 4;
      goto LAB_00089808;
    }
    uVar13 = uVar12;
    if (uVar12 == 9000 || uVar12 == 27000) {
      uVar6 = uVar12 >> 3;
      iVar10 = priv->fvco_tg << 1;
      uVar5 = iVar10 / uVar6;
      extraout_r1 = iVar10 % uVar6;
      cVar9 = '\0';
      uVar6 = (extraout_r1 << 0xc) / uVar6;
    }
    else {
      cVar9 = '\0';
      if (uVar12 != 0x34bc) goto LAB_00089804;
      uVar5 = (priv->fvco_tg * 4) / 0xd2f;
      uVar6 = (((priv->fvco_tg * 4) % 0xd2f) * 0x1000) / 0xd2f;
    }
  }
  uVar15 = (uVar5 << 0x12) >> 0x16;
  uVar14 = uVar5 & 0xf;
  if ((uVar6 & 0xfff) - 0x21 < 0xfbf) {
    reg_data = (uint8_t)uVar6;
    bVar16 = true;
    local_38 = (byte)((uVar6 << 0x14) >> 0x1c) | 0x30;
    uVar6 = uVar13 * (uVar6 & 0xfff) >> 0x10;
  }
  else {
    if ((uVar6 & 0x800) != 0) {
      uVar15 = (uVar5 + 1) * 0x40000 >> 0x16;
      uVar14 = uVar5 + 1 & 0xf;
    }
    bVar16 = false;
    reg_data = 0x0;
    local_38 = 0x30;
    uVar6 = 0;
  }
  uVar6 = uVar13 * uVar15 + uVar6 + (uVar13 * uVar14 >> 4);
  if (uVar6 < 8000000) {
    uVar6 = (uVar6 * -4 + 32000000) / 0x9eb1;
  }
  else {
    uVar6 = -(((uVar6 + 0x3f85ee00) * 4) / 0x9eb1);
  }
  uVar7 = uVar6 + 0x100;
  uVar17 = uVar7 * 0x80000 >> 0x18;
  uVar5 = (uVar17 & 1) + (uVar7 >> 6) & 0xff;
  if (uVar3 == uVar13) {
    uVar17 = uVar17 - ((int)(uVar6 << 0x1b) >> 0x1f);
    m88tc6800_write_reg(priv, 0x4e,0x11);
    uVar8 = 0x11;
LAB_000898b0:
    uVar5 = uVar17 & 0xff;
    m88tc6800_write_reg(priv, 0x4f,uVar8);
    m88tc6800_write_reg(priv, 0x4e,0x10);
    uVar8 = 0x8;
  }
  else {
    if (uVar13 == uVar12 / 3) {
      m88tc6800_write_reg(priv, 0x4e,0x11);
      m88tc6800_write_reg(priv, 0x4f,0x33);
      m88tc6800_write_reg(priv, 0x4e,0x10);
      m88tc6800_write_reg(priv, 0x4f,0xb);
      m88tc6800_write_reg(priv, 0x4e,0x15);
      uVar5 = uVar5 + (uVar17 - ((int)(uVar6 << 0x1b) >> 0x1f)) & 0xff;
      m88tc6800_write_reg(priv, 0x4f,0x43);
      goto LAB_00089584;
    }
    if (uVar13 == uVar3 / 3) {
      uVar12 = uVar7 * 0x100000 >> 0x18;
      m88tc6800_write_reg(priv, 0x4e,0x11);
      m88tc6800_write_reg(priv, 0x4f,0x33);
      m88tc6800_write_reg(priv, 0x4e,0x10);
      m88tc6800_write_reg(priv, 0x4f,0x8);
      m88tc6800_write_reg(priv, 0x4e,0x15);
      uVar5 = ((uVar17 + uVar12) - ((int)(uVar6 << 0x1c) >> 0x1f)) + (uVar12 & 1) & 0xff;
      m88tc6800_write_reg(priv, 0x4f,0x63);
      goto LAB_00089584;
    }
    if (uVar1 == uVar13) {
      uVar17 = (uVar7 >> 4) - ((int)(uVar6 << 0x1c) >> 0x1f);
      m88tc6800_write_reg(priv, 0x4e,0x11);
      uVar8 = 0x22;
      goto LAB_000898b0;
    }
    m88tc6800_write_reg(priv, 0x4e,0x11);
    m88tc6800_write_reg(priv, 0x4f,0x11);
    m88tc6800_write_reg(priv, 0x4e,0x10);
    uVar8 = 0xb;
  }
  m88tc6800_write_reg(priv, 0x4f,uVar8);
  m88tc6800_write_reg(priv, 0x4e,0x15);
  m88tc6800_write_reg(priv, 0x4f,0x13);
LAB_00089584:
  if (bVar16) {
    m88tc6800_write_reg(priv, 0x4e,0x12);
    uVar5 = uVar5 - 1 & 0xff;
    m88tc6800_write_reg(priv, 0x4f,0x21);
  }
  else {
    m88tc6800_write_reg(priv, 0x4e,0x12);
    m88tc6800_write_reg(priv, 0x4f,0x0);
  }
  if (cVar9 == '\x01') {
    uVar5 = uVar5 >> 1;
  }
  else if (cVar9 == '\x02') {
    uVar5 = uVar5 >> 2;
  }
  if (uVar5 < 2) {
    uVar5 = 2;
  }
  bVar11 = (byte)uVar5;
  if (0xe < uVar5) {
    bVar11 = 0xf;
  }
  m88tc6800_write_reg(priv, 0x4e,0xa);
  m88tc6800_write_reg(priv, 0x4f,0x63);
  m88tc6800_write_reg(priv, 0x4e,0xf);
  m88tc6800_write_reg(priv, 0x4f,0x3d);
  m88tc6800_write_reg(priv, 0x4e,0xb);
  m88tc6800_write_reg(priv, 0x4f,0xa1);
  m88tc6800_write_reg(priv, 0x3d,0xff);
  m88tc6800_write_reg(priv, 0x4e,0x1c);
  m88tc6800_write_reg(priv, 0x4f,0x7e);
  bVar2 = (byte)(uVar14 << 3);
  if (bVar16) {
    m88tc6800_write_reg(priv, 0x4e,0x19);
    m88tc6800_write_reg(priv, 0x4f,bVar2 | 3);
    m88tc6800_write_reg(priv, 0x4e,0x19);
    m88tc6800_write_reg(priv, 0x4f,bVar2 | 7);
  }
  else {
    m88tc6800_write_reg(priv, 0x4e,0x19);
    m88tc6800_write_reg(priv, 0x4f,bVar2 | 2);
    m88tc6800_write_reg(priv, 0x4e,0x19);
    m88tc6800_write_reg(priv, 0x4f,bVar2 | 6);
  }
  m88tc6800_write_reg(priv, 0x4e,0x17);
  m88tc6800_write_reg(priv, 0x4f,(byte)(uVar15 >> 8) | 0xc);
  m88tc6800_write_reg(priv, 0x4e,0x18);
  m88tc6800_write_reg(priv, 0x4f,(uint8_t)uVar15);
  m88tc6800_write_reg(priv, 0x4e,0x1a);
  m88tc6800_write_reg(priv, 0x4f,local_38);
  m88tc6800_write_reg(priv, 0x4e,0x1b);
  m88tc6800_write_reg(priv, 0x4f,reg_data);
  m88tc6800_write_reg(priv, 0x4e,0xe);
  m88tc6800_write_reg(priv, 0x4f,0x58);
  m88tc6800_write_reg(priv, 0x4e,0x14);
  m88tc6800_write_reg(priv, 0x4f,0x4c);
  m88tc6800_write_reg(priv, 0x4e,0x13);
  m88tc6800_write_reg(priv, 0x4f,bVar11 | 0x10);
}

void m88tc6800_set_bandwidth(struct m88tc6800_priv *priv, u32 bandwidth_hz)
{
    if (bandwidth_hz == 1700) {
        m88tc6800_write_reg(priv, 0x59, 0x09);
        m88tc6800_write_reg(priv, 0x29, 0xae);
        m88tc6800_write_reg(priv, 0x86, 0x3c);
    }
    else if (bandwidth_hz == 6000) {
        m88tc6800_write_reg(priv, 0x59, 0x0a);
        m88tc6800_write_reg(priv, 0x29, 0xaa);
        m88tc6800_write_reg(priv, 0x86, 0x12);
    }
    else if (bandwidth_hz == 7000) {
        m88tc6800_write_reg(priv, 0x59, 0x0b);
        m88tc6800_write_reg(priv, 0x29, 0xa8);
        m88tc6800_write_reg(priv, 0x86, 0x12);
    }
    else if (bandwidth_hz == 8000) {
        m88tc6800_write_reg(priv, 0x59, 0x0c);
        m88tc6800_write_reg(priv, 0x29, 0xa8);
        m88tc6800_write_reg(priv, 0x86, 0x12);
    }
    else {
        m88tc6800_write_reg(priv, 0x59, 0x0d);
        m88tc6800_write_reg(priv, 0x29, 0xa6);
        m88tc6800_write_reg(priv, 0x86, 0x12);
    }
}

void m88tc6800_set_dac(struct m88tc6800_priv *priv, u32 freq_khz, u32 bandwidth_hz)
{
  uint32_t uVar1 = priv->config.dac;
  uint extraout_r0 = freq_khz;
  int iVar2;
  int iVar3;
  uint uVar2;
  byte cVar4;
  int extraout_r1;
  int extraout_r1_00;
  uint uVar6;
  uint uVar7;
  
                    /* Unresolved local var: uint32_t DACFreq_KHz@[???]
                       Unresolved local var: uint32_t fc@[???]
                       Unresolved local var: uint32_t fadc@[???]
                       Unresolved local var: uint32_t fsd@[???]
                       Unresolved local var: uint32_t f2d@[???]
                       Unresolved local var: uint8_t tempnumber@[???]
                       Unresolved local var: uint8_t flt_bit@[???]
                       Unresolved local var: uint8_t gain_bit@[???]
                       Unresolved local var: uint8_t R_22@[???]
                       Unresolved local var: uint32_t FreqTrue108_Hz@[???]
                       Unresolved local var: uint32_t f1@[???]
                       Unresolved local var: uint32_t f2@[???]
                       Unresolved local var: uint32_t delta1@[???]
                       Unresolved local var: uint32_t Totalnum1@[???]
                       Unresolved local var: uint32_t cntT@[???]
                       Unresolved local var: uint32_t cntin@[???]
                       Unresolved local var: uint32_t NCOI@[???]
                       Unresolved local var: uint32_t z0@[???]
                       Unresolved local var: uint32_t z1@[???]
                       Unresolved local var: uint32_t z2@[???]
                       Unresolved local var: uint32_t tmp@[???]
                       Unresolved local var: uint32_t f1f2number@[???] */
  if (uVar1 < 0x1195) {
    cVar4 = 0x40;
  }
  else {
    cVar4 = 0x80;
  }
  m88tc6800_write_reg(priv, 0x22, cVar4 + priv->config.dac_gain * '\b' + 0x5);
  m88tc6800_set_reg_bits(priv, 0x20,0x3,0x5,0x6);
  if (priv->config.xtal == 24000) {
    m88tc6800_write_reg(priv, 0x4e,0x6);
    m88tc6800_write_reg(priv, 0x4f,0xd);
LAB_0008a204:
    if (extraout_r0 != 0xf37ff) {
      if (priv->config.xtal == 24000) {
        if (0x4a36 < (extraout_r0 + ((extraout_r0 >> 3) / 0xd2f) * -27000) - 0xfa1) {
          m88tc6800_write_reg(priv, 0x4e,0x6);
          m88tc6800_write_reg(priv, 0x4f,0x12);
          if (extraout_r0 % 30000 - 0xfa1 < 21999) {
            uVar7 = 120000;
            uVar6 = 120000000;
          }
          else {
            m88tc6800_write_reg(priv, 0x4e,0x6);
            m88tc6800_write_reg(priv, 0x4f,0x16);
            if (extraout_r0 % 0x7e90 - 0xfa1 < 0x5f4f) {
              uVar7 = 0x1fa40;
              uVar6 = 0x7b98a00;
            }
            else {
              m88tc6800_write_reg(priv, 0x4e,0x6);
              m88tc6800_write_reg(priv, 0x4f,0xf);
              if ((extraout_r0 + ((extraout_r0 >> 3) / 0xdc5) * -0x6e28) - 0xfa1 < 0x4ee7) {
                uVar7 = 0x1b8a0;
                uVar6 = 0x6b93100;
              }
              else {
                m88tc6800_write_reg(priv, 0x4e,0x6);
                m88tc6800_write_reg(priv, 0x4f,0x11);
                uVar7 = 0x1cb60;
                uVar6 = 0x7026f00;
              }
            }
          }
          goto LAB_0008a048;
        }
      }
      else if (priv->config.xtal == 27000) {
        if (extraout_r0 % 27000 - 0xfa1 < 18999) {
          uVar7 = 0x1a5e0;
          uVar6 = 108000000;
        }
        else {
          m88tc6800_write_reg(priv, 0x4e,0x6);
          m88tc6800_write_reg(priv, 0x4f,0xc);
          if (extraout_r0 % 0x7404 - 0xfa1 < 0x54c3) {
            uVar7 = 0x1d010;
            uVar6 = 0x714be80;
          }
          else {
            m88tc6800_write_reg(priv, 0x4e,0x6);
            m88tc6800_write_reg(priv, 0x4f,0x10);
            if (extraout_r0 % 0x7e90 - 0xfa1 < 0x5f4f) {
              uVar7 = 0x1fa40;
              uVar6 = 0x7b98a00;
            }
            else {
              m88tc6800_write_reg(priv, 0x4e,0x6);
              m88tc6800_write_reg(priv, 0x4f,0xa);
              if (extraout_r0 % 0x6ebe - 0xfa1 < 0x4f7d) {
                uVar7 = 0x1baf8;
                uVar6 = 0x6c258c0;
              }
              else {
                m88tc6800_write_reg(priv, 0x4e,0x6);
                m88tc6800_write_reg(priv, 0x4f,0xb);
                uVar7 = 0x1c584;
                uVar6 = 0x6eb8ba0;
              }
            }
          }
        }
        if (extraout_r0 == 0x79ac9) {
          m88tc6800_write_reg(priv, 0x4e,0x6);
          m88tc6800_write_reg(priv, 0x4f,0x4);
        }
        goto LAB_0008a048;
      }
    }
  }
  else if (priv->config.xtal == 27000) {
    m88tc6800_write_reg(priv, 0x4e,0x6);
    m88tc6800_write_reg(priv, 0x4f,0x8);
    goto LAB_0008a204;
  }
  uVar7 = 0x1a5e0;
  uVar6 = 108000000;
LAB_0008a048:
  extraout_r1 = (uVar1 * 0x7d000) % uVar6;
  iVar2 = (extraout_r1 << 5) / uVar6;
  iVar3 = (uVar1 * 0x7d000) / uVar6;
  iVar2 = (iVar2 + iVar3 * 0x20) * 4;
  m88tc6800_write_reg(priv, 0xfa,(uint8_t)((uint)iVar2 >> 8));
  m88tc6800_write_reg(priv, 0xfb,(uint8_t)iVar2);
  iVar2 = (bandwidth_hz >> 1) - 100;
  iVar2 = iVar2 * 0x1a5e0;
  iVar3 = iVar2 / uVar7;
  iVar2 = (0x1000 - iVar3) * 0x8000;
  uVar2 = (unsigned)iVar2 / iVar3;
  extraout_r1_00 = (unsigned)iVar2 % iVar3;
  m88tc6800_write_reg(priv, 0xe0,(byte)((uint)(extraout_r1_00 << 0x14) >> 0x1c));
  m88tc6800_write_reg(priv, 0xe1,(uint8_t)extraout_r1_00);
  m88tc6800_write_reg(priv, 0xe2,(uint8_t)((uint)iVar3 >> 8));
  m88tc6800_write_reg(priv, 0xe3,(uint8_t)iVar3);
  m88tc6800_write_reg(priv, 0xe4,(uint8_t)((ushort)(undefined2)uVar2 >> 8));
  m88tc6800_write_reg(priv, 0xe5,(uint8_t)(undefined2)uVar2);
  m88tc6800_write_reg(priv, 0xef,0x0);
  m88tc6800_write_reg(priv, 0xf0,0x0);
  m88tc6800_write_reg(priv, 0xf1,0x8);
  iVar2 = ((uint)(iVar3 << 0xf) >> 0xb) - 0x8000;
  m88tc6800_write_reg(priv, 0xf2,0x0);
  m88tc6800_write_reg(priv, 0xf3,(uint8_t)((uint)iVar2 >> 8));
  m88tc6800_write_reg(priv, 0xf4,(uint8_t)iVar2);
}

void m88tc6800_set_tune(struct m88tc6800_priv *priv, u32 freq_khz, u32 bandwidth_hz, u8 mixer_type, u32 wait_time_ms)
{
  m88tc6800_write_reg(priv, 0x04, 0x7f);
  m88tc6800_write_reg(priv, 0x05, 0xf8);
  m88tc6800_preset(priv);
  m88tc6800_set_rf_frontend(priv, freq_khz);
  m88tc6800_set_mixer(priv, freq_khz, 1);
  m88tc6800_set_lo(priv, freq_khz, 1);
  m88tc6800_set_pll(priv);
  m88tc6800_set_bandwidth(priv, bandwidth_hz);
  m88tc6800_write_reg(priv, 0x04, 0x00);
  m88tc6800_write_reg(priv, 0x05, 0x00);
  m88tc6800_set_dac(priv, freq_khz, bandwidth_hz);
  m88tc6800_write_reg(priv, 0x40, 0x1a);
  m88tc6800_write_reg(priv, 0x41, 0x00);
  m88tc6800_write_reg(priv, 0x44, 0x22);
  m88tc6800_write_reg(priv, 0x60, 0x34);
  m88tc6800_write_reg(priv, 0x05, 0x04);
  m88tc6800_write_reg(priv, 0xc2, 0x01);
  m88tc6800_write_reg(priv, 0x00, 0x01);
  m88tc6800_write_reg(priv, 0x00, 0x00);
  m88tc6800_write_reg(priv, 0xc2, 0x00);
  m88tc6800_write_reg(priv, 0x05, 0x00);
  m88tc6800_write_reg(priv, 0x39, 0x00);
  m88tc6800_write_reg(priv, 0x3a, 0x00);
  msleep(wait_time_ms);
}

void m88tc6800_set_poweron(struct m88tc6800_priv *priv, u32 bandwidth_hz)
{
    u8 reg_data;
    char cVar8;
    byte local_34;
    byte local_33;
    uint8_t local_32;

    m88tc6800_set_reg_bits(priv, 0x3c, 0x1, 0x0, 0x0);
    m88tc6800_set_reg_bits(priv, 0x55, 0x1, 0x0, 0x0);
    m88tc6800_write_reg(priv, 0x16, 0x80);
    m88tc6800_write_reg(priv, 0x17, 0xf3);
    if (priv->config.xtal_cap != 0x18) {
      m88tc6800_write_reg(priv, 0x1a, 0x4);
      m88tc6800_write_reg(priv, 0x1b, priv->config.xtal_cap);
      m88tc6800_write_reg(priv, 0x1a, 0x5);
      m88tc6800_write_reg(priv, 0x1b, priv->config.xtal_cap);
    }
    m88tc6800_write_reg(priv, 0xe, 0xa4);
    m88tc6800_write_reg(priv, 0x3, 0x0);
    m88tc6800_write_reg(priv, 0x4e, 0x2);
    m88tc6800_write_reg(priv, 0x4f, 0xe9);
    m88tc6800_write_reg(priv, 0x68, 0xf);
    m88tc6800_write_reg(priv, 0x69, 0xf1);
    m88tc6800_write_reg(priv, 0x76, 0x30);
    m88tc6800_write_reg(priv, 0x78, 0x25);
    m88tc6800_write_reg(priv, 0x79, 0x1f);
    m88tc6800_write_reg(priv, 0x7b, 0x0);
    m88tc6800_write_reg(priv, 0xb1, 0x2a);
    m88tc6800_write_reg(priv, 0xb2, 0xb2);
    m88tc6800_write_reg(priv, 0xb3, 0x54);
    m88tc6800_write_reg(priv, 0xb4, 0x6e);
    m88tc6800_write_reg(priv, 0xb5, 0x2a);
    m88tc6800_write_reg(priv, 0xb6, 0xb2);
    m88tc6800_write_reg(priv, 0xba, 0x90);
    m88tc6800_write_reg(priv, 0x27, 0xb4);
    m88tc6800_write_reg(priv, 0x25, 0x2a);
    m88tc6800_write_reg(priv, 0x84, 0x20);
    m88tc6800_set_reg_bits(priv, 0xfc, 0x1, 0x5, 0x5);
    m88tc6800_set_reg_bits(priv, 0x2a, 0x1, 0x0, 0x1);
    m88tc6800_set_reg_bits(priv, 0xa7, 0x0, 0x0, 0x2);
    m88tc6800_write_reg(priv, 0x2c, 0xa);
    m88tc6800_write_reg(priv, 0xc7, 0x2);
    m88tc6800_write_reg(priv, 0x5, 0x4);
    m88tc6800_write_reg(priv, 0xc6, 0x80);
    m88tc6800_write_reg(priv, 0x5, 0x0);
    m88tc6800_write_reg(priv, 0xad, 0xa);
    m88tc6800_write_reg(priv, 0xfd, 0x1a);
    m88tc6800_write_reg(priv, 0x5, 0x4);
    m88tc6800_write_reg(priv, 0xc7, 0x0);
    m88tc6800_write_reg(priv, 0x5, 0x0);
    m88tc6800_write_reg(priv, 0x39, 0x5);
    m88tc6800_write_reg(priv, 0x3a, 0x3f);
    m88tc6800_write_reg(priv, 0x39, 0x6);
    m88tc6800_write_reg(priv, 0x3a, 0x3);
    m88tc6800_write_reg(priv, 0x39, 0x7);
    m88tc6800_write_reg(priv, 0x3a, 0xd);
    m88tc6800_write_reg(priv, 0x39, 0x8);
    m88tc6800_write_reg(priv, 0x3a, 0x9);
    m88tc6800_write_reg(priv, 0x39, 0x9);
    m88tc6800_write_reg(priv, 0x3a, 0x0);
    m88tc6800_write_reg(priv, 0x39, 0xa);
    m88tc6800_write_reg(priv, 0x3a, 0x0);
    m88tc6800_write_reg(priv, 0x39, 0xb);
    m88tc6800_write_reg(priv, 0x3a, 0x1);
    m88tc6800_write_reg(priv, 0x39, 0xc);
    m88tc6800_write_reg(priv, 0x3a, 0x0);
    m88tc6800_write_reg(priv, 0x39, 0xd);
    m88tc6800_write_reg(priv, 0x3a, 0x7f);
    m88tc6800_write_reg(priv, 0x39, 0x1a);
    m88tc6800_write_reg(priv, 0x3a, 0x8);
    m88tc6800_write_reg(priv, 0x39, 0x1b);
    m88tc6800_write_reg(priv, 0x3a, 0xd);
    m88tc6800_write_reg(priv, 0x39, 0xe);
    m88tc6800_write_reg(priv, 0x3a, 0x4);
    m88tc6800_write_reg(priv, 0x39, 0xf);
    m88tc6800_write_reg(priv, 0x3a, 0x4);
    m88tc6800_write_reg(priv, 0x39, 0x10);
    m88tc6800_write_reg(priv, 0x3a, 0x1);
    m88tc6800_write_reg(priv, 0x4e, 0x2b);
    m88tc6800_write_reg(priv, 0x4f, 0xa);
    m88tc6800_write_reg(priv, 0x4e, 0x2c);
    m88tc6800_write_reg(priv, 0x4f, 0x10);
    m88tc6800_write_reg(priv, 0x4e, 0x2f);
    m88tc6800_write_reg(priv, 0x4f, 0x1);
    m88tc6800_write_reg(priv, 0x4e, 0x29);
    m88tc6800_write_reg(priv, 0x4f, 0xcc);
    m88tc6800_write_reg(priv, 0x7c, 0x10);
    m88tc6800_set_reg_bits(priv, 0x35, 0x0, 0x4, 0x7);
    m88tc6800_set_reg_bits(priv, 0x37, 0x6, 0x0, 0x2);
    m88tc6800_set_reg_bits(priv, 0xbd, 0x1, 0x7, 0x7);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x40);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x41);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x42);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x4a);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x52);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x5a);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0x62);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0xa2);
    m88tc6800_write_reg(priv, 0xb8, 0x0);
    m88tc6800_write_reg(priv, 0xbe, 0xe2);
    m88tc6800_write_reg(priv, 0xb8, 0x1);
    m88tc6800_write_reg(priv, 0xbe, 0x22);
    m88tc6800_write_reg(priv, 0xb8, 0x1);
    m88tc6800_write_reg(priv, 0xbe, 0x62);
    m88tc6800_write_reg(priv, 0xb8, 0x3);
    m88tc6800_write_reg(priv, 0xbe, 0x62);
    m88tc6800_write_reg(priv, 0xb8, 0x5);
    m88tc6800_write_reg(priv, 0xbe, 0x62);
    m88tc6800_write_reg(priv, 0xb8, 0x5);
    m88tc6800_write_reg(priv, 0xbe, 0xa2);
    m88tc6800_write_reg(priv, 0xb8, 0x7);
    m88tc6800_write_reg(priv, 0xbe, 0xa2);
    m88tc6800_write_reg(priv, 0xb8, 0x7);
    m88tc6800_write_reg(priv, 0xbe, 0xaa);
    m88tc6800_write_reg(priv, 0xb8, 0x7);
    m88tc6800_write_reg(priv, 0xbe, 0xb2);
    m88tc6800_write_reg(priv, 0xb8, 0x7);
    m88tc6800_write_reg(priv, 0xbe, 0xb3);
    m88tc6800_write_reg(priv, 0xa0, 0xc);
    m88tc6800_write_reg(priv, 0x39, 0x6a);
    m88tc6800_write_reg(priv, 0x3a, 0x3f);
    m88tc6800_set_reg_bits(priv, 0x1d, 0x0, 0x1, 0x1);
    m88tc6800_write_reg(priv, 0x4e, 0x5);
    m88tc6800_write_reg(priv, 0x4f, 0xc0);
    m88tc6800_write_reg(priv, 0x4e, 0x5);
    m88tc6800_write_reg(priv, 0x4f, 0x80);
    m88tc6800_write_reg(priv, 0x45, 0x5d);
    m88tc6800_set_tune(priv, 0xf37ff, bandwidth_hz, 0x32, 1);
    m88tc6800_write_reg(priv, 0x14, 0x7b);
    m88tc6800_write_reg(priv, 0x12, 0x1e);
    m88tc6800_set_reg_bits(priv, 0xbd, 0x1, 0x4, 0x4);
    m88tc6800_set_reg_bits(priv, 0xbc, 0x3, 0x6, 0x7);
    m88tc6800_expand_reg_bits(priv, 0x53, 0xa, 0x0, 0x7, 0x7);
    m88tc6800_expand_reg_bits(priv, 0x53, 0xd, 0x0, 0x7, 0x7);
    m88tc6800_expand_reg_bits(priv, 0x53, 0x14, 0x1, 0x6, 0x6);
    m88tc6800_write_reg(priv, 0x53, 0x3);
    m88tc6800_write_reg(priv, 0x54, 0x5a);
    m88tc6800_write_reg(priv, 0x53, 0x4);
    m88tc6800_write_reg(priv, 0x54, 0xc4);
    cVar8 = '\x04';
    do {
      m88tc6800_write_reg(priv, 0x53, 0x1);
      m88tc6800_write_reg(priv, 0x54, 0x0);
      m88tc6800_write_reg(priv, 0x53, 0x0);
      m88tc6800_write_reg(priv, 0x54, 0xff);
      m88tc6800_write_reg(priv, 0x53, 0x0);
      cVar8 = cVar8 + -1;
      m88tc6800_write_reg(priv, 0x54, 0x0);
      m88tc6800_expand_reg_bits(priv, 0x53, 0x2, 0x1, 0x6, 0x6);
      m88tc6800_expand_reg_bits(priv, 0x53, 0x2, 0x0, 0x6, 0x6);
      m88tc6800_write_reg(priv, 0x53, 0x1);
      m88tc6800_write_reg(priv, 0x54, 0x3a);
      m88tc6800_write_reg(priv, 0x53, 0x6);
      m88tc6800_read_reg(priv, 0x54, &local_34);
      m88tc6800_write_reg(priv, 0x53, 0x8);
      m88tc6800_read_reg(priv, 0x54, &local_33);
      m88tc6800_read_reg(priv, 0x50, &local_32);
      if (((local_34 - 0x2c < 3) && (local_33 - 0x2c < 3)) && ((byte)(local_32 + 0x3b) < 4)) break;
    } while (cVar8 != '\0');
    m88tc6800_write_reg(priv, 0x53, 0x1);
    m88tc6800_write_reg(priv, 0x54, 0x0);
    m88tc6800_write_reg(priv, 0x12, 0xe);
    m88tc6800_set_reg_bits(priv, 0xbd, 0x0, 0x4, 0x4);
    m88tc6800_write_reg(priv, 0x53, 0xa);
    m88tc6800_write_reg(priv, 0x54, 0x80);
    m88tc6800_write_reg(priv, 0x53, 0xb);
    m88tc6800_write_reg(priv, 0x54, 0x0);
    m88tc6800_write_reg(priv, 0x53, 0xc);
    m88tc6800_write_reg(priv, 0x54, 0x0);
    m88tc6800_write_reg(priv, 0x53, 0xd);
    m88tc6800_write_reg(priv, 0x54, 0x80);
    m88tc6800_write_reg(priv, 0x53, 0xe);
    m88tc6800_write_reg(priv, 0x54, 0x0);
    m88tc6800_write_reg(priv, 0x53, 0xf);
    m88tc6800_write_reg(priv, 0x54, 0x0);
    if (priv->config.xtal == 24000) {
      m88tc6800_set_reg_bits(priv, 0x5b, 0x2, 0x3, 0x5);
    }
    else if (priv->config.xtal == 27000) {
      m88tc6800_set_reg_bits(priv, 0x5b, 0x6, 0x3, 0x5);
      m88tc6800_expand_reg_bits(priv, 0x4e, 0x8, 0x1, 0x7, 0x7);
      m88tc6800_set_tune(priv, 0x79ac9, bandwidth_hz, 0x32, 1);
      m88tc6800_write_reg(priv, 0x39, 0x77);
      m88tc6800_read_reg(priv, 0x3a, &reg_data);
      m88tc6800_write_reg(priv, 0x39, 0x77);
      m88tc6800_write_reg(priv, 0x3a, (reg_data & 0x3f) | 0x40);
    }
    m88tc6800_write_reg(priv, 0x45, 0x5d);
    m88tc6800_set_reg_bits(priv, 0x3c, 0x1, 0x7, 0x7);
}

int m88tc6800_set_freq(struct m88tc6800_priv *priv, u32 freq_khz, u32 bandwidth_hz)
{
    u8 reg_data;

    m88tc6800_read_reg(priv, 0x3e, &reg_data);
    reg_data = (reg_data & 0x1e) >> 1;
    if (reg_data != 0) {
        m88tc6800_set_reg_bits(priv, 0x3c, 0, 7, 7);
        m88tc6800_set_reg_bits(priv, 0x3e, 0, 1, 4);
    }

    m88tc6800_read_reg(priv, 0x07, &reg_data);
    if (reg_data & 0x8) {
        m88tc6800_write_reg(priv, 0x07, 0x0d);
        m88tc6800_write_reg(priv, 0x11, 0x00);
    }
    else {
        m88tc6800_write_reg(priv, 0x0c, 0x63);
        m88tc6800_write_reg(priv, 0x07, 0x0d);
        m88tc6800_write_reg(priv, 0x0c, 0xe3);
        m88tc6800_write_reg(priv, 0x11, 0x00);
    }

    m88tc6800_read_reg(priv, 0x3c, &reg_data);
    reg_data &= 0x80;
    if (reg_data == 0) {
        m88tc6800_set_poweron(priv, bandwidth_hz);
    }
    else {
        m88tc6800_write_reg(priv, 0x45, 0x5d);
    }
    m88tc6800_write_reg(priv, 0x04, 0x7f);
    m88tc6800_write_reg(priv, 0x05, 0xd8);
    m88tc6800_preset(priv);
    m88tc6800_set_rf_frontend(priv, freq_khz);
    m88tc6800_write_reg(priv, 0x05, 0xf8);
    m88tc6800_set_mixer(priv, freq_khz, m88tc6800_get_mixer_type(priv, freq_khz));
    m88tc6800_set_lo(priv, freq_khz, m88tc6800_get_mixer_type(priv, freq_khz));
    m88tc6800_set_pll(priv);
    m88tc6800_set_bandwidth(priv, bandwidth_hz);
    m88tc6800_write_reg(priv, 0x04, 0x00);
    m88tc6800_write_reg(priv, 0x05, 0x00);
    m88tc6800_read_reg(priv, 0x3e, &reg_data); // TODO: not needed
    m88tc6800_set_dac(priv, freq_khz, bandwidth_hz);
    m88tc6800_write_reg(priv, 0xc9, 0x05);
    m88tc6800_write_reg(priv, 0x40, 0x1a);
    m88tc6800_write_reg(priv, 0x41, 0x00);
    if (priv->config.xtal == 24000) {
        m88tc6800_write_reg(priv, 0x44, 0x22);
    }
    else if (priv->config.xtal == 27000) {
        m88tc6800_write_reg(priv, 0x44, 0x2a);
    }
    m88tc6800_write_reg(priv, 0x60, 0x34);
    m88tc6800_write_reg(priv, 0x05, 0x04);
    m88tc6800_write_reg(priv, 0xc2, 0x01);
    m88tc6800_write_reg(priv, 0x00, 0x01);
    m88tc6800_write_reg(priv, 0x00, 0x00);
    m88tc6800_write_reg(priv, 0xc2, 0x00);
    m88tc6800_write_reg(priv, 0x05, 0x00);
    m88tc6800_write_reg(priv, 0x39, 0x00);
    m88tc6800_write_reg(priv, 0x3a, 0x00);
    m88tc6800_write_reg(priv, 0x0d, 0xc4);
    m88tc6800_write_reg(priv, 0x3d, 0xfe);
    {
        u32 tmp1, tmp2, tmp3;
        m88tc6800_read_reg(priv, 0x4a, &reg_data);
        tmp1 = reg_data;
        m88tc6800_read_reg(priv, 0x4b, &reg_data);
        tmp2 = reg_data;
        m88tc6800_read_reg(priv, 0x4c, &reg_data);
        tmp3 = (((tmp1 & 0xf) << 8) + tmp2) >> ((reg_data << 25) >> 30);
        if (priv->config.xtal == 27000) {
            tmp3 = 1000 * (tmp3 * 216) / 24;
        }
        else {
            tmp3 = 1000 * tmp3 * 8;
        }
        if ((tmp3 - freq_khz <= 20000) || (freq_khz - tmp3 <= 20000) || (tmp3 - 3 * freq_khz <= 20000) || (3 * freq_khz - tmp3 <= 20000)) {
            m88tc6800_write_reg(priv, 0x10, 0x50);
        }
    }
    m88tc6800_write_reg(priv, 0x66, 0x48);
    m88tc6800_write_reg(priv, 0xb0, 0x0b);
    m88tc6800_write_reg(priv, 0x60, 0x34);
    return 0;
}

static uint32_t xm_list_dBm[] = {
       0x0,           0x0,         0x25A,         0x3BA,
     0x4B4,         0x576,         0x614,         0x69A,
     0x70E,         0x774,         0x7D0,         0x823,
     0x86E,         0x8B4,         0x8F4,         0x930,
     0x968,         0x99D,         0x9CF,         0x9FE,
     0xA2A,         0xA54,         0xA7D,         0xAA3,
     0xAC8,         0xAEC,         0xB0E,         0xB2F,
     0xB4E,         0xB6D,         0xB8A,         0xBA7,
     0xBC2,         0xBDD,         0xBF7,         0xC10,
     0xC29,         0xC40,         0xC58,         0xC6E,
     0xC84,         0xC9A,         0xCAE,         0xCC3,
     0xCD7,         0xCEA,         0xCFE,         0xD10,
     0xD22,         0xD34,         0xD46,         0xD57,
     0xD68,         0xD79,         0xD89,         0xD99,
     0xDA8,         0xDB8,         0xDC7,         0xDD6,
     0xDE4,         0xDF3,         0xE01,         0xE0F,
     0xE1C,         0xE2A,         0xE37,         0xE44,
     0xE51,         0xE5E,         0xE6A,         0xE77,
     0xE83,         0xE8F,         0xE9A,         0xEA6,
     0xEB2,         0xEBD,         0xEC8,         0xED3,
     0xEDE,         0xEE9,         0xEF4,         0xEFE,
     0xF09,         0xF13,         0xF1D,         0xF27,
     0xF31,         0xF3B,         0xF44,         0xF4E,
     0xF58,         0xF61,         0xF6A,         0xF73,
     0xF7D,         0xF86,         0xF8E,         0xF97,
     0xFA0,         0xFA9,         0xFB1,         0xFBA,
     0xFC2,         0xFCA,         0xFD3,         0xFDB,
     0xFE3,         0xFEB,         0xFF3,         0xFFB,
    0x1002,        0x100A,        0x1012,        0x1019,
    0x1021,        0x1028,        0x1030,        0x1037,
    0x103E,        0x1046,        0x104D,        0x1054,
    0x105B,        0x1062,        0x1069,        0x1070,
    0x1076,        0x107D,        0x1084,        0x108B,
    0x1091,        0x1098,        0x109E,        0x10A5,
    0x10AB,        0x10B1,        0x10B8,        0x10BE,
    0x10C4,        0x10CA,        0x10D1,        0x10D7,
    0x10DD,        0x10E3,        0x10E9,        0x10EF,
    0x10F5,        0x10FA,        0x1100,        0x1106,
    0x110C,        0x1111,        0x1117,        0x111D,
    0x1122,        0x1128,        0x112D,        0x1133,
    0x1138,        0x113E,        0x1143,        0x1148,
    0x114E,        0x1153,        0x1158,        0x115D,
    0x1163,        0x1168,        0x116D,        0x1172,
    0x1177,        0x117C,        0x1181,        0x1186,
    0x118B,        0x1190,        0x1195,        0x119A,
    0x119F,        0x11A3,        0x11A8,        0x11AD,
    0x11B2,        0x11B6,        0x11BB,        0x11C0,
    0x11C4,        0x11C9,        0x11CE,        0x11D2,
    0x11D7,        0x11DB,        0x11E0,        0x11E4,
    0x11E9,        0x11ED,        0x11F1,        0x11F6,
    0x11FA,        0x11FE,        0x1203,        0x1207,
    0x120B,        0x1210,        0x1214,        0x1218,
    0x121C,        0x1220,        0x1224,        0x1229,
    0x122D,        0x1231,        0x1235,        0x1239,
    0x123D,        0x1241,        0x1245,        0x1249,
    0x124D,        0x1251,        0x1255,        0x1259,
    0x125C,        0x1260,        0x1264,        0x1268,
    0x126C,        0x1270,        0x1273,        0x1277,
    0x127B,        0x127F,        0x1282,        0x1286,
    0x128A,        0x128D,        0x1291,        0x1295,
    0x1298,        0x129C,        0x12A0,        0x12A3,
    0x12A7,        0x12AA,        0x12AE,        0x12B1,
    0x12B5,        0x12B8,        0x12BC,        0x12BF,
    0x12C3,        0x12C6,        0x12CA,        0x12CD
};

int m88tc6800_get_rf_power(struct m88tc6800_priv *priv, s32 *strength)
{
  uint32_t uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  bool bVar5;
  byte local_31 [5];

                    /* Unresolved local var: uint32_t ret@[???]
                       Unresolved local var: int32_t rfLevel@[???] */
                    /* Unresolved local var: uint32_t ret@[???]
                       Unresolved local var: char[22] __func__@[???] */
                    /* Unresolved local var: uint32_t ret@[???]
                       Unresolved local var: int32_t rflevel@[???]
                       Unresolved local var: uint8_t buf@[???]
                       Unresolved local var: uint8_t bufH@[???]
                       Unresolved local var: uint8_t bufL@[???]
                       Unresolved local var: int32_t level@[???]
                       Unresolved local var: int32_t gain_x10@[???]
                       Unresolved local var: int32_t gain_ref_x10@[???]
                       Unresolved local var: int32_t pd_x10@[???]
                       Unresolved local var: int32_t pdtref_x10@[???]
                       Unresolved local var: uint32_t freq_KHz@[???]
                       Unresolved local var: uint32_t[16][16] xm_list_dBm@[???] */
  uint32_t freq_khz = priv->frequency_hz / 1000;
  uVar1 = freq_khz;
  if (freq_khz < 0x1adb1) {
    m88tc6800_read_reg(priv, 0x63,local_31);
    uVar2 = (uint)local_31[0];
    m88tc6800_read_reg(priv, 0x8b,local_31);
    iVar4 = uVar2 * 10 + -0x1d1;
    iVar3 = uVar2 * 10 + -0x13b;
    local_31[0] = local_31[0] & 0x77;
LAB_0008e5cc:
    if (local_31[0] == 0x55) {
      iVar3 = iVar4 + 0x82;
    }
    else {
joined_r0x0008e5dc:
      if (local_31[0] == 0x11) {
        if (uVar1 < 0xc3501) {
          iVar3 = iVar3 + -0x28;
        }
        else {
          iVar3 = iVar3 + -0x19;
        }
      }
    }
  }
  else if (freq_khz < 0x30d41) {
    m88tc6800_read_reg(priv, 0x63,local_31);
    uVar2 = (uint)local_31[0];
    iVar3 = uVar2 * 10 + -300;
    m88tc6800_read_reg(priv, 0x8b,local_31);
    local_31[0] = local_31[0] & 0x77;
    if (local_31[0] != 0x55) goto joined_r0x0008e5dc;
    iVar3 = uVar2 * 10 + -0x140;
  }
  else {
    if (freq_khz < 0x518b1) {
      iVar4 = -0x1ae;
    }
    else if (freq_khz < 0x61a81) {
      iVar4 = -0x1ea;
    }
    else if (freq_khz < 0x927c1) {
      iVar4 = -0x1d6;
    }
    else if (freq_khz < 0xaae61) {
      iVar4 = -0x1c7;
    }
    else {
      if (800000 < freq_khz) {
        if (freq_khz < 0xdbba1) {
          m88tc6800_read_reg(priv, 0x63,local_31);
          uVar2 = (uint)local_31[0];
          iVar4 = uVar2 * 10 + -0x1cc;
          m88tc6800_read_reg(priv, 0x8b,local_31);
          iVar3 = uVar2 * 10 + -0x136;
          local_31[0] = local_31[0] & 0x77;
          goto LAB_0008e5cc;
        }
        m88tc6800_read_reg(priv, 0x63,local_31);
        iVar3 = (short)(ushort)local_31[0] * 10 + -0x154;
        m88tc6800_read_reg(priv, 0x8b,local_31);
        local_31[0] = local_31[0] & 0x77;
        goto LAB_0008e4e4;
      }
      iVar4 = -0x1c2;
    }
    m88tc6800_read_reg(priv, 0x63,local_31);
    iVar4 = (short)(ushort)local_31[0] * 10 + iVar4;
    m88tc6800_read_reg(priv, 0x8b,local_31);
    iVar3 = iVar4 + 0x96;
    local_31[0] = local_31[0] & 0x77;
    if (uVar1 < 0xdbba1) goto LAB_0008e5cc;
  }
LAB_0008e4e4:
  if (7999 < uVar1 - 0x595b1) {
    if (uVar1 - 480000 < 0x2ee1) goto LAB_0008e764;
    if (14000 < uVar1 - 610000) {
      if (20000 < uVar1 - 800000) goto LAB_0008e508;
      goto LAB_0008e75c;
    }
  }
  else if (65999 < uVar1 - 0x518b1) {
    if (73999 < uVar1 - 0x61a81) {
      if (((uVar1 - 0x73b91 < 0x1ec30) || (uVar1 - 0x927c1 < 200000)) || (uVar1 - 0xc3501 < 58000))
      {
        iVar3 = iVar3 + 0x41;
        goto LAB_0008e508;
      }
      if (41999 < uVar1 - 0xd1791) {
        if (uVar1 - 0xdbba1 < 120000) {
          iVar3 = iVar3 + 0xf;
        }
        else {
          bVar5 = uVar1 == 110000;
          if (uVar1 < 0x1adb1) {
            bVar5 = 0;
          }
          if (bVar5) {
            iVar3 = iVar3 + -0x1e;
          }
        }
        goto LAB_0008e508;
      }
LAB_0008e75c:
      iVar3 = iVar3 + 10;
      goto LAB_0008e508;
    }
LAB_0008e764:
    iVar3 = iVar3 + 0x32;
    goto LAB_0008e508;
  }
  iVar3 = iVar3 + 0x3c;
LAB_0008e508:
  m88tc6800_read_reg(priv, 0xc8,local_31);
  iVar3 = (int)((xm_list_dBm[local_31[0]] / 10 - 0x1a9) - iVar3) / 10;
  iVar4 = -0x6b;
  if (-0x6c < iVar3) {
    iVar4 = iVar3;
  }
  *strength = iVar4;
  return 0;
}
