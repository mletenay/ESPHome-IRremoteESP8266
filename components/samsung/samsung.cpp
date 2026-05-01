#include "samsung.h"

namespace esphome
{
    namespace samsung
    {
        static const char *const TAG = "samsung.climate";

        void SamsungClimate::setup()
        {
            climate_ir::ClimateIR::setup();
            this->apply_state();
        }

        void SamsungClimate::transmit_state()
        {
            this->apply_state();
            this->send();
        }

        void SamsungClimate::send()
        {
            if (this->mode == climate::CLIMATE_MODE_OFF)
            {
                this->ac_.sendOff();
            }
            else
            {
                this->ac_.send();
            }
        }

        void SamsungClimate::apply_state()
        {
            if (this->mode == climate::CLIMATE_MODE_OFF)
            {
                this->ac_.off();
            }
            else
            {
                this->ac_.setTemp(static_cast<uint8_t>(this->target_temperature));

                switch (this->mode)
                {
                case climate::CLIMATE_MODE_HEAT_COOL:
                    this->ac_.setMode(kSamsungAcAuto);
                    break;
                case climate::CLIMATE_MODE_COOL:
                    this->ac_.setMode(kSamsungAcCool);
                    break;
                case climate::CLIMATE_MODE_DRY:
                    this->ac_.setMode(kSamsungAcDry);
                    break;
                case climate::CLIMATE_MODE_FAN_ONLY:
                    this->ac_.setMode(kSamsungAcFan);
                    break;
                case climate::CLIMATE_MODE_HEAT:
                    this->ac_.setMode(kSamsungAcHeat);
                    break;
                default:
                    this->ac_.setMode(kSamsungAcAuto);
                    break;
                }

                if (this->fan_mode.has_value())
                {
                    switch (this->fan_mode.value())
                    {
                    case climate::CLIMATE_FAN_AUTO:
                        this->ac_.setFan(this->mode == climate::CLIMATE_MODE_HEAT_COOL ? kSamsungAcFanAuto2 : kSamsungAcFanAuto);
                        break;
                    case climate::CLIMATE_FAN_LOW:
                        this->ac_.setFan(kSamsungAcFanLow);
                        break;
                    case climate::CLIMATE_FAN_MEDIUM:
                        this->ac_.setFan(kSamsungAcFanMed);
                        break;
                    case climate::CLIMATE_FAN_HIGH:
                        this->ac_.setFan(kSamsungAcFanHigh);
                        break;
                    default:
                        this->ac_.setFan(kSamsungAcFanAuto);
                        break;
                    }
                }

                if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL)
                {
                    this->ac_.setSwing(true);
                }
                else
                {
                    this->ac_.setSwing(false);
                }

                this->ac_.on();
            }

            ESP_LOGI(TAG, "%s", this->ac_.toString().c_str());
        }

    } // namespace samsung
} // namespace esphome
