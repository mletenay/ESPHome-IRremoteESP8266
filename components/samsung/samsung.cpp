#include "samsung.h"

// Define Samsung AC timing constants locally if not available from IRremoteESP8266
#ifndef kSamsungAcHdrMark
const uint16_t kSamsungAcHdrMark = 690;
const uint16_t kSamsungAcHdrSpace = 17844;
const uint16_t kSamsungAcSectionMark = 3086;
const uint16_t kSamsungAcSectionSpace = 8864;
const uint16_t kSamsungAcSectionGap = 2886;
const uint16_t kSamsungAcBitMark = 586;
const uint16_t kSamsungAcOneSpace = 1432;
const uint16_t kSamsungAcZeroSpace = 436;
const uint16_t kSamsungAcSectionLength = 7;
#endif
namespace esphome
{
    namespace samsung
    {
        static const char *const TAG = "samsung.climate";

        void SamsungClimate::setup()
        {
            IrRemoteBase::setup();
            apply_state();
        }

        void SamsungClimate::transmit_state()
        {
            apply_state();
            send();
        }

        void SamsungClimate::send()
        {
            uint8_t *message = ac_.getRaw();

            auto send_section = [&](const uint8_t *section, uint16_t header_mark, uint32_t header_space)
            {
                sendGeneric(
                    header_mark, header_space,
                    kSamsungAcBitMark, kSamsungAcOneSpace,
                    kSamsungAcBitMark, kSamsungAcZeroSpace,
                    kSamsungAcBitMark, kSamsungAcSectionGap,
                    section, kSamsungAcSectionLength,
                    38000);
            };

            send_section(message, kSamsungAcHdrMark, kSamsungAcHdrSpace);
            send_section(message + kSamsungAcSectionLength, kSamsungAcSectionMark, kSamsungAcSectionSpace);
            send_section(message + 2 * kSamsungAcSectionLength, kSamsungAcSectionMark, kSamsungAcSectionSpace);
        }

        void SamsungClimate::apply_state()
        {
            if (mode == climate::CLIMATE_MODE_OFF)
            {
                ac_.off();
            }
            else
            {
                ac_.setTemp(static_cast<uint8_t>(target_temperature));

                switch (mode)
                {
                case climate::CLIMATE_MODE_HEAT_COOL:
                    ac_.setMode(kSamsungAcAuto);
                    break;
                case climate::CLIMATE_MODE_COOL:
                    ac_.setMode(kSamsungAcCool);
                    break;
                case climate::CLIMATE_MODE_DRY:
                    ac_.setMode(kSamsungAcDry);
                    break;
                case climate::CLIMATE_MODE_FAN_ONLY:
                    ac_.setMode(kSamsungAcFan);
                    break;
                case climate::CLIMATE_MODE_HEAT:
                    ac_.setMode(kSamsungAcHeat);
                    break;
                default:
                    ac_.setMode(kSamsungAcAuto);
                    break;
                }

                if (fan_mode.has_value())
                {
                    switch (fan_mode.value())
                    {
                    case climate::CLIMATE_FAN_AUTO:
                        ac_.setFan(mode == climate::CLIMATE_MODE_HEAT_COOL ? kSamsungAcFanAuto2 : kSamsungAcFanAuto);
                        break;
                    case climate::CLIMATE_FAN_LOW:
                        ac_.setFan(kSamsungAcFanLow);
                        break;
                    case climate::CLIMATE_FAN_MEDIUM:
                        ac_.setFan(kSamsungAcFanMed);
                        break;
                    case climate::CLIMATE_FAN_HIGH:
                        ac_.setFan(kSamsungAcFanHigh);
                        break;
                    default:
                        ac_.setFan(kSamsungAcFanAuto);
                        break;
                    }
                }

                bool swing_h = false;
                bool swing_v = false;
                switch (swing_mode)
                {
                case climate::CLIMATE_SWING_HORIZONTAL:
                    swing_h = true;
                    break;
                case climate::CLIMATE_SWING_BOTH:
                    swing_h = true;
                    swing_v = true;
                    break;
                case climate::CLIMATE_SWING_VERTICAL:
                    swing_v = true;
                    break;
                default:
                    break;
                }

                ac_.setSwingH(swing_h);
                ac_.setSwing(swing_v);
                ac_.on();
            }

            ESP_LOGI(TAG, "%s", ac_.toString().c_str());
        }

    } // namespace samsung
} // namespace esphome
