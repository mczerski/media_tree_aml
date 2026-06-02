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
            priv->int_ldiv = 0x80;
        }
        else if (freq_khz < 83000) {
            reg_data = 0x7a;
            priv->int_ldiv = 0x60;
        }
        else if (freq_khz < 125000) {
            reg_data = 0x0a;
            priv->int_ldiv = 0x40;
        }
        else if (freq_khz < 167000) {
            reg_data = 0x0d;
            priv->int_ldiv = 0x30;
        }
        else if (freq_khz < 250000) {
            reg_data = 0x09;
            priv->int_ldiv = 0x20;
        }
        else if (freq_khz < 335000) {
            reg_data = 0x0c;
            priv->int_ldiv = 0x18;
        }
        else {
            reg_data = 0x08;
            priv->int_ldiv = 0x10;
        }
    }
    else if (freq_khz < 250000 || mixer_type != 3) {
        if (freq_khz < 56000) {
            reg_data = 0x0b;
            priv->int_ldiv = 0x80;
        }
        else if (freq_khz < 83000) {
            reg_data = 0x7a;
            priv->int_ldiv = 0x60;
        }
        else if (freq_khz < 125000) {
            reg_data = 0x0a;
            priv->int_ldiv = 0x40;
        }
        else if (freq_khz < 167000) {
            reg_data = 0x79;
            priv->int_ldiv = 0x30;
        }
        else if (freq_khz < 250000) {
            reg_data = 0x09;
            priv->int_ldiv = 0x20;
        }
        else if (freq_khz < 335000) {
            reg_data = 0x78;
            priv->int_ldiv = 0x18;
        }
        else if (freq_khz < 500000) {
            reg_data = 0x08;
            priv->int_ldiv = 0x10;
        }
        else if (freq_khz < 667000) {
            reg_data = 0x74;
            priv->int_ldiv = 0x0c;
        }
        else {
            reg_data = 0x04;
            priv->int_ldiv = 0x08;
        }
    }
    else {
        if (freq_khz < 333000) {
            reg_data = 0xf8;
            priv->int_ldiv = 0x18;
        }
        else if (freq_khz < 500000) {
            reg_data = 0x88;
            priv->int_ldiv = 0x10;
        }
        else if (freq_khz < 666000) {
            reg_data = 0x74;
            priv->int_ldiv = 0x0c;
        }
        else {
            reg_data = 0x04;
            priv->int_ldiv = 0x08;
        }
    }
    priv->fvco_tg = freq_khz * priv->int_ldiv;
    if (priv->fvco_tg < 6700000) {
        m88tc6800_set_reg_bits(priv, 0x13, 4, 3, 5);
    }
    else {
        m88tc6800_set_reg_bits(priv, 0x13, 7, 3, 5);
    }
    m88tc6800_write_reg(priv, 0x14, reg_data);
}

/* Refactored from Ghidra decompilation of _mt_fe_tn_set_PLL_freq_tc6800_cxd2856.
 *
 * Variable name mapping from decompilation:
 *   uVar4  -> fvco_tg        (priv->fvco_tg)
 *   uVar3  -> crystal        (priv->config.xtal)
 *   uVar1  -> crystal_half   (crystal / 2)
 *   uVar12 -> crystal_x2     (crystal * 2)
 *   uVar13 -> pll_ref        (selected PLL reference frequency, kHz)
 *   uVar5  -> fdiv_n         (integer+fractional divider word: bits[11:4]=upper, [3:0]=lower)
 *   uVar6  -> fdiv_f / fvco  (fractional part during divider calc, reused for Fvco_KHz later)
 *   uVar14 -> fdiv_n_lo      (FDIV_N[3:0])
 *   uVar15 -> fdiv_n_hi      (FDIV_N[11:4])
 *   cVar9  -> spur_opti      (spur_opti_by_dsm: 0=none, 1=>>1, 2=>>2)
 *   bVar16 -> sdm_en         (pll_sdm_en: fractional mode enabled)
 *   reg_data / local_38      (registers 0x1b / 0x1a: fractional low/high bytes)
 *   uVar6  -> fvco_khz       (Fvco in kHz, after divider reconstruction)
 *   uVar7  -> icp_raw        (ICP intermediate: fvco_khz + 0x100)
 *   uVar17 -> icp_hi         (ICP high-resolution scaled value)
 *   uVar5  -> icp_code       (ICP register byte, reused from fdiv_n)
 *   bVar11 -> icp_code_clamped
 *   bVar2  -> fdiv_n_lo_shifted (uVar14 << 3, for reg 0x19)
 *   iVar10 -> nearest_mult   (nearest 27000-multiple of fvco_tg, for 27 MHz crystal)
 */
void m88tc6800_set_pll(struct m88tc6800_priv *priv)
{
    uint32_t crystal    = priv->config.xtal;
    uint32_t fvco_tg    = priv->fvco_tg;
    uint32_t crystal_half = crystal >> 1;       /* crystal / 2  */
    uint32_t crystal_x2   = crystal * 2;        /* crystal * 2  */

    /* PLL reference divider selection and FDIV_N / FDIV_F computation.
     * pll_ref  : selected reference clock fed to the PLL (kHz)
     * fdiv_n   : combined integer+fractional divider word
     * fdiv_f   : 12-bit fractional part  (bits [11:0] of fdiv_n word)
     * spur_opti: downstream ICP right-shift selector (0/1/2)
     */
    uint32_t pll_ref;
    uint32_t fdiv_n;
    uint32_t fdiv_f;
    uint8_t  spur_opti = 0;

    if (crystal == 24000) {
        /* ----------------------------------------------------------------
         * 24 MHz crystal.
         * Check whether fvco_tg / ldiv falls in the upper Fvco band
         * (above 0x518b1 + 0x176e = 339999 kHz ≈ 340 MHz boundary).
         * If so, use 48 MHz reference; otherwise 24 MHz.
         * Reference step for 24/48 MHz:  ref >> 4  (i.e. ref / 16)
         * but for the straight 24 MHz path the step is 0x5dc (1500).
         * ---------------------------------------------------------------- */
        uint32_t ldiv_quotient = fvco_tg / priv->int_ldiv;

        if (ldiv_quotient - 0x518b1U > 0x176e) {
            pll_ref = 48000;
            fdiv_n  = fvco_tg / (pll_ref >> 4);
            fdiv_f  = (fvco_tg % (pll_ref >> 4)) << 12;
            fdiv_f /= (pll_ref >> 4);
        } else {
            uint32_t ref_step = 0x5dc; /* 1500 kHz */

            pll_ref = 24000;
            fdiv_n  = fvco_tg / ref_step;
            fdiv_f  = (fvco_tg % ref_step) << 12;
            fdiv_f /= ref_step;
        }

    } else if (crystal == 27000) {
        /* ----------------------------------------------------------------
         * 27 MHz crystal.
         * Compute nearest multiple of 27000 to fvco_tg using the
         * compiler-emitted multiply-high approximation for division by
         * 27000:   round(x / 27000) = x/27000 - sign_correction
         * The result (iVar10) is the nearest 27000-multiple.
         *
         * Depending on how far fvco_tg sits from that multiple:
         *   ±10 .. ±1010 kHz  → pll_ref = 18000, spur_opti = 2
         *   +1012 .. +2020 kHz → pll_ref = 18000, spur_opti = 1
         *   within -2020..-1012 kHz → pll_ref = 18000, spur_opti = 1 (via cVar9=true=1)
         *   within ±10 kHz of multiple (no spur) → pll_ref = 54000
         *   otherwise → pll_ref = 18000 or 54000
         * ---------------------------------------------------------------- */

        /* Compiler multiply-high trick: round(fvco_tg / 27000) * 27000 */
        uint32_t q27 = fvco_tg / 27000;
        int32_t  sign_corr = (int32_t)(
            ((uint64_t)(fvco_tg >> 2) * 0x4dac1b9dULL >> 32) << 21
        ) >> 31;
        int32_t  nearest_mult = (int32_t)(q27 - (uint32_t)sign_corr) * 27000;

        if (((uint32_t)nearest_mult + 10U < fvco_tg &&
             fvco_tg < (uint32_t)nearest_mult + 0x3f2U) ||
            (fvco_tg < (uint32_t)nearest_mult - 10U &&
             (uint32_t)nearest_mult - 0x3f2U < fvco_tg)) {
            spur_opti = 2;
            pll_ref   = 18000;
        } else if ((uint32_t)nearest_mult + 0x3fcU < fvco_tg &&
               fvco_tg < (uint32_t)nearest_mult + 0x7e4U) {
            spur_opti = 1;
            pll_ref   = 18000;
        } else if (fvco_tg < (uint32_t)nearest_mult - 0x3fcU) {
            /* spur_opti = (iVar10 - 0x7e4 < fvco_tg) — i.e. 0 or 1 */
            spur_opti = ((uint32_t)nearest_mult - 0x7e4U < fvco_tg) ? 1 : 0;
            pll_ref   = (spur_opti) ? 18000 : 54000;
        } else {
            spur_opti = 0;
            pll_ref   = 54000;
        }

        uint32_t ref_step = pll_ref >> 4;

        fdiv_n = fvco_tg / ref_step;
        fdiv_f = (fvco_tg % ref_step) << 12;
        fdiv_f /= ref_step;

    } else if (crystal_x2 == 9000 || crystal_x2 == 27000) {
        /* ----------------------------------------------------------------
         * crystal = 4500 Hz or 13500 kHz  (crystal*2 == 9000 or 27000).
         * ref_step = crystal_x2 / 8
         * fdiv_n derived from fvco_tg * 2
         * ---------------------------------------------------------------- */
        uint32_t ref_step = crystal_x2 >> 3;
        uint32_t fvco_x2  = fvco_tg << 1;

        pll_ref   = crystal_x2;
        fdiv_n    = fvco_x2 / ref_step;
        fdiv_f    = (fvco_x2 % ref_step) << 12;
        fdiv_f   /= ref_step;
        spur_opti = 0;

    } else if (crystal_x2 == 0x34bc) {
        /* ----------------------------------------------------------------
         * crystal*2 == 13500 (0x34bc = 13500) — special fixed divider.
         * ref divisor = 0xd2f (3375)
         * ---------------------------------------------------------------- */
        pll_ref   = crystal_x2;
        fdiv_n    = (fvco_tg * 4) / 0xd2fU;
        fdiv_f    = ((fvco_tg * 4) % 0xd2fU) * 0x1000U / 0xd2fU;
        spur_opti = 0;

    } else {
        /* ----------------------------------------------------------------
         * Fallback: use the same ref_step as the 27 MHz / 48 MHz path.
         * pll_ref stays crystal_x2; ref_step = pll_ref >> 4.
         * ---------------------------------------------------------------- */
        pll_ref   = crystal_x2;
        uint32_t ref_step = pll_ref >> 4;

        fdiv_n    = fvco_tg / ref_step;
        fdiv_f    = (fvco_tg % ref_step) << 12;
        fdiv_f   /= ref_step;
        spur_opti = 0;
    }

    /* ------------------------------------------------------------------
     * Split fdiv_n into upper (bits [11:4]) and lower nibble (bits [3:0]).
     * fdiv_n_hi : 8-bit field written to regs 0x17/0x18
     * fdiv_n_lo : 4-bit field written (shifted left 3) to reg 0x19
     * ------------------------------------------------------------------ */
    uint32_t fdiv_n_hi = (fdiv_n << 0x12) >> 0x16;  /* bits [11:4] → 8 bits */
    uint32_t fdiv_n_lo = fdiv_n & 0xf;               /* bits [3:0]           */

    /* ------------------------------------------------------------------
     * SDM (fractional) enable decision.
     * The fractional part fdiv_f is valid when its 12-bit value lies in
     * [0x21 .. 0x21 + 0xfbf - 1]  i.e.  [33 .. 4128].
     * Outside that range the PLL runs in integer mode; if the fractional
     * part rounds up (bit 11 set), increment the integer divider by 1.
     * ------------------------------------------------------------------ */
    bool     sdm_en;
    uint8_t  fdiv_frac_hi;  /* reg 0x1a */
    uint8_t  fdiv_frac_lo;  /* reg 0x1b */
    uint32_t frac_contribution;

    if ((fdiv_f & 0xfffU) - 0x21U < 0xfbfU) {
        fdiv_frac_lo     = (uint8_t)fdiv_f;
        sdm_en           = true;
        fdiv_frac_hi     = (uint8_t)((fdiv_f << 0x14) >> 0x1c) | 0x30;
        frac_contribution = (pll_ref * (fdiv_f & 0xfffU)) >> 16;
    } else {
        if (fdiv_f & 0x800U) {
            /* Round up the integer divider */
            fdiv_n_hi = (fdiv_n + 1) * 0x40000U >> 0x16;
            fdiv_n_lo = (fdiv_n + 1) & 0xfU;
        }
        sdm_en            = false;
        fdiv_frac_lo      = 0x00;
        fdiv_frac_hi      = 0x30;
        frac_contribution = 0;
    }

    /* ------------------------------------------------------------------
     * Reconstruct Fvco_KHz from the (now possibly rounded) divider and
     * convert to an ICP current code.
     *
     * Fvco_KHz = pll_ref * fdiv_n_hi + frac_contribution
     *            + (pll_ref * fdiv_n_lo) / 16
     *
     * ICP formula (linear in Fvco):
     *   if Fvco < 8000 MHz:  icp = (32000000 - Fvco_KHz * 4)  / 0x9eb1
     *   else:                icp = -((Fvco_KHz + 0x3f85ee00) * 4) / 0x9eb1
     * ------------------------------------------------------------------ */
    uint32_t fvco_khz = pll_ref * fdiv_n_hi + frac_contribution
                + (pll_ref * fdiv_n_lo >> 4);

    uint32_t icp;
    if (fvco_khz < 8000000U) {
        icp = (fvco_khz * -4 + 32000000U) / 0x9eb1U;
    } else {
        icp = -(((fvco_khz + 0x3f85ee00U) * 4U) / 0x9eb1U);
    }

    uint32_t icp_raw  = icp + 0x100U;
    uint32_t icp_hi   = icp_raw * 0x80000U >> 24;  /* high-res ICP scaled value */
    uint32_t icp_code = ((icp_hi & 1U) + (icp_raw >> 6)) & 0xffU;

    /* ------------------------------------------------------------------
     * Reference divider path selection — programs registers 0x10/0x11/0x15
     * and adjusts icp_code / icp_hi based on which sub-divider is in use.
     * ------------------------------------------------------------------ */
    if (crystal == pll_ref) {
        /* Direct crystal reference (÷1) */
        icp_hi = icp_hi - (uint32_t)((int32_t)(icp << 0x1b) >> 31);

        m88tc6800_write_reg(priv, 0x4e, 0x11);
        m88tc6800_write_reg(priv, 0x4f, 0x11);
        m88tc6800_write_reg(priv, 0x4e, 0x10);
        m88tc6800_write_reg(priv, 0x4f, 0x08);
        m88tc6800_write_reg(priv, 0x4e, 0x15);
        m88tc6800_write_reg(priv, 0x4f, 0x13);

        icp_code = icp_hi & 0xffU;

    } else if (pll_ref == crystal_x2 / 3) {
        /* ÷2/3 reference path */
        icp_hi   = icp_hi - (uint32_t)((int32_t)(icp << 0x1b) >> 31);
        icp_code = (icp_code + icp_hi) & 0xffU;

        m88tc6800_write_reg(priv, 0x4e, 0x11);
        m88tc6800_write_reg(priv, 0x4f, 0x33);
        m88tc6800_write_reg(priv, 0x4e, 0x10);
        m88tc6800_write_reg(priv, 0x4f, 0x0b);
        m88tc6800_write_reg(priv, 0x4e, 0x15);
        m88tc6800_write_reg(priv, 0x4f, 0x43);

    } else if (pll_ref == crystal / 3) {
        /* ÷3 reference path */
        uint32_t icp_x3   = icp_raw * 0x100000U >> 24;
        uint32_t icp_x3_r = icp_x3 & 1U;

        icp_code = ((icp_hi + icp_x3)
                - (uint32_t)((int32_t)(icp << 0x1c) >> 31)
                + icp_x3_r) & 0xffU;

        m88tc6800_write_reg(priv, 0x4e, 0x11);
        m88tc6800_write_reg(priv, 0x4f, 0x33);
        m88tc6800_write_reg(priv, 0x4e, 0x10);
        m88tc6800_write_reg(priv, 0x4f, 0x08);
        m88tc6800_write_reg(priv, 0x4e, 0x15);
        m88tc6800_write_reg(priv, 0x4f, 0x63);

    } else if (crystal_half == pll_ref) {
        /* ÷2 reference path */
        icp_hi   = (icp_raw >> 4)
               - (uint32_t)((int32_t)(icp << 0x1c) >> 31);
        icp_code = icp_hi & 0xffU;

        m88tc6800_write_reg(priv, 0x4e, 0x11);
        m88tc6800_write_reg(priv, 0x4f, 0x22);
        m88tc6800_write_reg(priv, 0x4e, 0x10);
        m88tc6800_write_reg(priv, 0x4f, 0x08);
        m88tc6800_write_reg(priv, 0x4e, 0x15);
        m88tc6800_write_reg(priv, 0x4f, 0x13);

    } else {
        /* Default / other reference ratio */
        m88tc6800_write_reg(priv, 0x4e, 0x11);
        m88tc6800_write_reg(priv, 0x4f, 0x11);
        m88tc6800_write_reg(priv, 0x4e, 0x10);
        m88tc6800_write_reg(priv, 0x4f, 0x0b);
        m88tc6800_write_reg(priv, 0x4e, 0x15);
        m88tc6800_write_reg(priv, 0x4f, 0x13);
    }

    /* ------------------------------------------------------------------
     * SDM enable register (0x12) and ICP scale factor.
     * When SDM is active, decrement icp_code by 1 and signal the hardware.
     * ------------------------------------------------------------------ */
    m88tc6800_write_reg(priv, 0x4e, 0x12);
    if (sdm_en) {
        icp_code = (icp_code - 1U) & 0xffU;
        m88tc6800_write_reg(priv, 0x4f, 0x21);
    } else {
        m88tc6800_write_reg(priv, 0x4f, 0x00);
    }

    /* Apply spur optimisation shift to ICP code */
    if (spur_opti == 1)
        icp_code >>= 1;
    else if (spur_opti == 2)
        icp_code >>= 2;

    /* Clamp ICP code to [2 .. 15] */
    if (icp_code < 2U)
        icp_code = 2U;
    uint8_t icp_code_clamped = (icp_code > 0xeU) ? 0xfU : (uint8_t)icp_code;

    /* ------------------------------------------------------------------
     * Write remaining PLL registers.
     * ------------------------------------------------------------------ */

    /* Fixed configuration registers */
    m88tc6800_write_reg(priv, 0x4e, 0x0a);
    m88tc6800_write_reg(priv, 0x4f, 0x63);
    m88tc6800_write_reg(priv, 0x4e, 0x0f);
    m88tc6800_write_reg(priv, 0x4f, 0x3d);
    m88tc6800_write_reg(priv, 0x4e, 0x0b);
    m88tc6800_write_reg(priv, 0x4f, 0xa1);
    m88tc6800_write_reg(priv, 0x3d, 0xff);
    m88tc6800_write_reg(priv, 0x4e, 0x1c);
    m88tc6800_write_reg(priv, 0x4f, 0x7e);

    /* Reg 0x19: FDIV_N lower nibble (shifted left 3) + VCO calibration trigger bits.
     * SDM mode: write 0x03 first (arm), then 0x07 (trigger).
     * Integer mode: write 0x02 first (arm), then 0x06 (trigger).            */
    uint8_t fdiv_n_lo_shifted = (uint8_t)(fdiv_n_lo << 3);

    m88tc6800_write_reg(priv, 0x4e, 0x19);
    m88tc6800_write_reg(priv, 0x4f, fdiv_n_lo_shifted | (sdm_en ? 0x03 : 0x02));
    m88tc6800_write_reg(priv, 0x4e, 0x19);
    m88tc6800_write_reg(priv, 0x4f, fdiv_n_lo_shifted | (sdm_en ? 0x07 : 0x06));

    /* Regs 0x17/0x18: FDIV_N upper byte (high bits ORed with 0x0c, low byte) */
    m88tc6800_write_reg(priv, 0x4e, 0x17);
    m88tc6800_write_reg(priv, 0x4f, (uint8_t)(fdiv_n_hi >> 8) | 0x0c);
    m88tc6800_write_reg(priv, 0x4e, 0x18);
    m88tc6800_write_reg(priv, 0x4f, (uint8_t)fdiv_n_hi);

    /* Regs 0x1a/0x1b: fractional divider high/low */
    m88tc6800_write_reg(priv, 0x4e, 0x1a);
    m88tc6800_write_reg(priv, 0x4f, fdiv_frac_hi);
    m88tc6800_write_reg(priv, 0x4e, 0x1b);
    m88tc6800_write_reg(priv, 0x4f, fdiv_frac_lo);

    /* Reg 0x0e/0x14: fixed charge pump / LPF settings */
    m88tc6800_write_reg(priv, 0x4e, 0x0e);
    m88tc6800_write_reg(priv, 0x4f, 0x58);
    m88tc6800_write_reg(priv, 0x4e, 0x14);
    m88tc6800_write_reg(priv, 0x4f, 0x4c);

    /* Reg 0x13: ICP code with mode flag in bit 4 */
    m88tc6800_write_reg(priv, 0x4e, 0x13);
    m88tc6800_write_reg(priv, 0x4f, icp_code_clamped | 0x10);
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

/* Refactored from Ghidra decompilation of _mt_fe_tn_set_DAC_tc6800_cxd2856.
 *
 * Variable name mapping from decompilation:
 *   uVar1         -> dac            (priv->config.dac, DAC/ADC clock in kHz)
 *   extraout_r0   -> freq_KHz       (function argument, never modified)
 *   cVar4         -> flt_base       (0x40 or 0x80 depending on DAC rate)
 *   uVar7         -> fadc_khz       (selected ADC sample rate, kHz)
 *   uVar6         -> fadc_hz        (selected ADC sample rate, Hz = fadc_khz * 1000)
 *   iVar3         -> fc             (filter centre coefficient)
 *   iVar2         -> ncoi / bw_val  (reused: NCO word, then bandwidth intermediate)
 *   uVar2         -> z1             (filter coefficient z1 = z0 / fc)
 *   extraout_r1   -> ncoi_rem       (remainder of dac*0x7d000 / fadc_hz)
 *   extraout_r1_00 -> z0_frac       (remainder of z0 / fc, fractional bits of z1)
 */
/* Tuner application type codes */
#define TUNER_APP_DVB_T   0x1
#define TUNER_APP_ISDB_T  0x3
#define TUNER_APP_DVB_C   0x4

void m88tc6800_set_dac(struct m88tc6800_priv *priv, uint32_t freq_KHz, uint32_t bandwidth_hz)
{
    uint32_t dac     = priv->config.dac;
    uint32_t crystal = priv->config.xtal;
    uint32_t application = 0;

    /* ------------------------------------------------------------------
     * Reg 0x22: DAC filter bandwidth / gain control.
     * flt_base selects the upper or lower half of the register range
     * based on the DAC clock rate (threshold 0x1195 = 4501 kHz).
     * tuner_dac_gain is scaled by 8 (the '\b' literal = 8) and added.
     * ------------------------------------------------------------------ */
    uint8_t flt_base = (dac < 0x1195U) ? 0x40U : 0x80U;
    m88tc6800_write_reg(priv, 0x22,
                flt_base + (uint8_t)(priv->config.dac_gain * 8U) + 0x05U);

    m88tc6800_set_reg_bits(priv, 0x20, 0x3, 0x5, 0x6);

    /* ------------------------------------------------------------------
     * ADC sample-rate selection (fadc_khz / fadc_hz) and reg 0x06 flt_bit.
     *
     * The outer if/else sets a crystal-dependent default for reg 0x06,
     * then both 24 MHz and 27 MHz paths share the same inner dispatch
     * (the original goto LAB_0008a204 merges them).
     *
     * Special case: freq_KHz == 0xf37ff (997375 kHz) skips the inner
     * dispatch entirely and falls through to the default fadc values.
     *
     * The inner dispatch picks fadc_khz / fadc_hz and writes reg 0x06
     * according to which spectral spur band freq_KHz falls in.
     * Each band test is an unsigned "distance from nearest harmonic < N"
     * check done via: (freq % period) - offset < window  (unsigned arith,
     * so values below 'offset' wrap and are large → fail the test).
     * ------------------------------------------------------------------ */

    /* Default ADC rate (108 MHz); overridden below when needed */
    uint32_t fadc_khz = 0x1a5e0U;     /* 108000 kHz */
    uint32_t fadc_hz  = 108000000U;    /* 108000000 Hz */

    if (crystal == 24000U) {
        m88tc6800_write_reg(priv, 0x4e, 0x06);
        m88tc6800_write_reg(priv, 0x4f, 0x0d);
    } else if (crystal == 27000U) {
        m88tc6800_write_reg(priv, 0x4e, 0x06);
        m88tc6800_write_reg(priv, 0x4f, 0x08);
    }

    /* Inner dispatch — shared by both crystal paths */
    if (freq_KHz != 0xf37ffU) {
        if (crystal == 24000U) {
            /* 24 MHz crystal spur avoidance bands.
             * First gate: check distance from nearest 27 MHz harmonic
             * using the approximation  freq % 27000 ≈
             *   freq - (freq >> 3) / 0xd2f * 27000            */
            uint32_t dist24 = freq_KHz
                      + (freq_KHz >> 3) / 0xd2fU * (uint32_t)-27000
                      - 0xfa1U;
            if (dist24 > 0x4a36U) {
                m88tc6800_write_reg(priv, 0x4e, 0x06);
                m88tc6800_write_reg(priv, 0x4f, 0x12);

                if (freq_KHz % 30000U - 0xfa1U < 21999U) {
                    /* 30 MHz band */
                    fadc_khz = 120000U;
                    fadc_hz  = 120000000U;
                } else {
                    m88tc6800_write_reg(priv, 0x4e, 0x06);
                    m88tc6800_write_reg(priv, 0x4f, 0x16);

                    if (freq_KHz % 0x7e90U - 0xfa1U < 0x5f4fU) {
                        /* 130 kHz band (0x7e90 = 32400) */
                        fadc_khz = 0x1fa40U;   /* 129600 kHz */
                        fadc_hz  = 0x7b98a00U; /* 129,960,960 Hz */
                    } else {
                        m88tc6800_write_reg(priv, 0x4e, 0x06);
                        m88tc6800_write_reg(priv, 0x4f, 0x0f);

                        /* approx freq % 0x6e28 (28200) */
                        uint32_t dist24b = freq_KHz
                                   + (freq_KHz >> 3) / 0xdc5U * (uint32_t)-0x6e28U
                                   - 0xfa1U;
                        if (dist24b < 0x4ee7U) {
                            /* 113 kHz band */
                            fadc_khz = 0x1b8a0U;   /* 112800 kHz */
                            fadc_hz  = 0x6b93100U; /* 112,980,736 Hz */
                        } else {
                            m88tc6800_write_reg(priv, 0x4e, 0x06);
                            m88tc6800_write_reg(priv, 0x4f, 0x11);
                            fadc_khz = 0x1cb60U;   /* 118624 kHz */
                            fadc_hz  = 0x7026f00U; /* 117,350,144 Hz */
                        }
                    }
                }
            }
            /* else: no spur conflict — keep default fadc 108 MHz */

        } else if (crystal == 27000U) {
            /* 27 MHz crystal spur avoidance bands */
            if (freq_KHz % 27000U - 0xfa1U < 18999U) {
                /* 27 MHz harmonic — default 108 MHz ADC rate is fine */
                fadc_khz = 0x1a5e0U;
                fadc_hz  = 108000000U;
            } else {
                m88tc6800_write_reg(priv, 0x4e, 0x06);
                m88tc6800_write_reg(priv, 0x4f, 0x0c);

                if (freq_KHz % 0x7404U - 0xfa1U < 0x54c3U) {
                    /* 0x7404 = 29700 kHz band */
                    fadc_khz = 0x1d010U;   /* 118800 kHz */
                    fadc_hz  = 0x714be80U; /* 118,964,352 Hz */
                } else {
                    m88tc6800_write_reg(priv, 0x4e, 0x06);
                    m88tc6800_write_reg(priv, 0x4f, 0x10);

                    if (freq_KHz % 0x7e90U - 0xfa1U < 0x5f4fU) {
                        /* 0x7e90 = 32400 kHz band */
                        fadc_khz = 0x1fa40U;   /* 129600 kHz */
                        fadc_hz  = 0x7b98a00U; /* 129,960,960 Hz */
                    } else {
                        m88tc6800_write_reg(priv, 0x4e, 0x06);
                        m88tc6800_write_reg(priv, 0x4f, 0x0a);

                        if (freq_KHz % 0x6ebeU - 0xfa1U < 0x4f7dU) {
                            /* 0x6ebe = 28350 kHz band */
                            fadc_khz = 0x1baf8U;   /* 113400 kHz */
                            fadc_hz  = 0x6c258c0U; /* 113,540,288 Hz */
                        } else {
                            m88tc6800_write_reg(priv, 0x4e, 0x06);
                            m88tc6800_write_reg(priv, 0x4f, 0x0b);
                            fadc_khz = 0x1c584U;   /* 116100 kHz */
                            fadc_hz  = 0x6eb8ba0U; /* 116,523,936 Hz */
                        }
                    }
                }
            }

            /* Special override for freq 0x79ac9 (498,377 kHz) */
            if (freq_KHz == 0x79ac9U) {
                m88tc6800_write_reg(priv, 0x4e, 0x06);
                m88tc6800_write_reg(priv, 0x4f, 0x04);
            }
        }
    }

    /* ------------------------------------------------------------------
     * NCO (numerically controlled oscillator) register calculation.
     *
     * NCOI = ( (dac * 0x7d000) / fadc_hz * 32
     *        + (dac * 0x7d000) % fadc_hz * 32 / fadc_hz ) * 4
     *
     * Written as a 16-bit value to regs 0xfa (high byte) / 0xfb (low byte).
     * ------------------------------------------------------------------ */
    uint32_t dac_scaled  = dac * 0x7d000U;
    uint32_t ncoi_int    = dac_scaled / fadc_hz;
    uint32_t ncoi_rem    = dac_scaled % fadc_hz;
    uint32_t ncoi        = (ncoi_int * 0x20U + (ncoi_rem << 5) / fadc_hz) * 4U;

    m88tc6800_write_reg(priv, 0xfa, (uint8_t)(ncoi >> 8));
    m88tc6800_write_reg(priv, 0xfb, (uint8_t)ncoi);

    /* ------------------------------------------------------------------
     * Filter coefficient (fc) calculation.
     *
     * bw_scaled is derived from tuner_bandwidth and tuner_application:
     *   application 3 (ISDB-T):  bw_scaled = (bw/2 - 150) * 108000
     *   application 4 (DVB-C):   bw_scaled = 418,000,000  when bw == 8000 kHz
     *                             bw_scaled = (bw/2 - 100) * 108000  otherwise
     *   application 1 (DVB-T):   bw_scaled = (bw/2 - 140) * 108000
     *   default:                  bw_scaled = (bw/2 - 100) * 108000
     *
     * fc = bw_scaled / fadc_khz
     * ------------------------------------------------------------------ */
    uint32_t bw_scaled;

    if (application == TUNER_APP_ISDB_T) {
        bw_scaled = ((bandwidth_hz >> 1) - 0x96U) * 0x1a5e0U;
    } else if (application == TUNER_APP_DVB_C) {
        if (bandwidth_hz == 8000U)
            bw_scaled = 0x18c89ac0U;   /* 418,000,000 — fixed for 8 MHz DVB-C */
        else
            bw_scaled = ((bandwidth_hz >> 1) - 100U) * 0x1a5e0U;
    } else {
        uint32_t bw_offset = (application == TUNER_APP_DVB_T)
                     ? 0x8cU   /* 140 kHz */
                     : 100U;
        bw_scaled = ((bandwidth_hz >> 1) - bw_offset) * 0x1a5e0U;
    }

    uint32_t fc      = bw_scaled / fadc_khz;

    /* ------------------------------------------------------------------
     * Filter register values derived from fc.
     *
     * z0      = (0x1000 - fc) * 0x8000
     * z1      = z0 / fc            (regs 0xe4/0xe5, low 16 bits)
     * z0_frac = z0 % fc            (fractional bits written to 0xe0/0xe1)
     * fc      written to regs 0xe2/0xe3
     * f3val   = fc * 16 - 0x8000   (regs 0xf3/0xf4)
     * ------------------------------------------------------------------ */
    uint32_t z0      = (0x1000U - fc) * 0x8000U;
    uint32_t z1      = z0 / fc;
    uint32_t z0_frac = z0 % fc;

    m88tc6800_write_reg(priv, 0xe0, (uint8_t)((z0_frac << 0x14) >> 0x1c));
    m88tc6800_write_reg(priv, 0xe1, (uint8_t)z0_frac);
    m88tc6800_write_reg(priv, 0xe2, (uint8_t)(fc >> 8));
    m88tc6800_write_reg(priv, 0xe3, (uint8_t)fc);
    m88tc6800_write_reg(priv, 0xe4, (uint8_t)((uint16_t)z1 >> 8));
    m88tc6800_write_reg(priv, 0xe5, (uint8_t)(uint16_t)z1);

    m88tc6800_write_reg(priv, 0xef, 0x00);
    m88tc6800_write_reg(priv, 0xf0, 0x00);
    m88tc6800_write_reg(priv, 0xf1, 0x08);

    /* f3val = (fc << 4) - 0x8000
     * Original: ((uint)(iVar3 << 0xf) >> 0xb) - 0x8000
     *         = (fc * 0x8000 * 2 >> 11) - 0x8000
     *         = (fc << 4) - 0x8000                      */
    uint32_t f3val = ((uint32_t)(fc << 0xf) >> 0xb) - 0x8000U;

    m88tc6800_write_reg(priv, 0xf2, 0x00);
    m88tc6800_write_reg(priv, 0xf3, (uint8_t)(f3val >> 8));
    m88tc6800_write_reg(priv, 0xf4, (uint8_t)f3val);
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

    m88tc6800_write_reg(priv, 0x45, 0x5d);
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
