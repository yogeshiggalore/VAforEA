/*******************************************************************************
 * File Name: voice_ctrl.cpp
 *
 * Version: 1.0
 *
 * Description:
 * This is header file for voice control module in VAforEA project
 *
 *
 *
 ********************************************************************************
 * VAforEA (2024-25)
 ********************************************************************************/

#include <Arduino.h>
#include <voice_ctrl.h>

const char* vc_cmds[] = {"ONOFF", "SET_VOLT", "UP_VOLT", "DWN_VOLT", "SET_CURR", "UP_CURR", "DWN_CURR", "ACTION"};
const char* vc_errs[] = {"ERR_PORT", "ERR_PKT_FRT", "ERR_CMD"};

// Constructors ////////////////////////////////////////////////////////////////
voice_ctrl::voice_ctrl() {}

int voice_ctrl::begin(struct voice_config *vc_cfg, struct voice_data *vc_data)
{
    int status=0;

    cfg = vc_cfg;
    data = vc_data;

    switch (vc_cfg->port)
    {
    case SELECT_UART_PORT_ZERO:
        Serial.begin(115200);
        cfg->port = SELECT_UART_PORT_ZERO;
        break;
    
    case SELECT_UART_PORT_ONE:
        Serial1.begin(115200);
        cfg->port = SELECT_UART_PORT_ONE;
        break;

    case SELECT_UART_PORT_TWO:
        Serial2.begin(115200);
        cfg->port = SELECT_UART_PORT_TWO;
        break;

    default:
        status = -1;
        cfg->port = SELECT_UART_PORT_NONE;
        cfg->cb_on_cmd_rx = NULL;
        cfg->cb_on_err = NULL;
        data->err = VOICE_CTRL_ERR_PORT;
        break;
    }
    
    if( status == 0 )
    {
        data->err = VOICE_CTRL_ERR_NONE;
        rx_cntr = 0;
        data->cmd = VOICE_CTRL_CMD_NONE;
    }
    else
    {
        if( cfg->cb_on_err != nullptr )
        {
            cfg->cb_on_err(data->err);
        }
    }

    return status;
}

int voice_ctrl::read(void)
{
    if( data->err == VOICE_CTRL_ERR_NONE )
    {

        switch ( cfg->port )
        {
        case SELECT_UART_PORT_ZERO:
            while( Serial.available() )
            {
                if( rx_cntr >= RX_BUFFER_SIZE )
                {
                    rx_cntr = 0;
                }
                rx_buf[rx_cntr] = Serial.read();
                rx_cntr++;
            }
            break;

        case SELECT_UART_PORT_ONE:
            while( Serial1.available() )
            {
                if( rx_cntr >= RX_BUFFER_SIZE )
                {
                    rx_cntr = 0;
                }
                rx_buf[rx_cntr] = Serial1.read();
                rx_cntr++;
            }
            break;

        case SELECT_UART_PORT_TWO:
            while( Serial2.available() )
            {
                if( rx_cntr >= RX_BUFFER_SIZE )
                {
                    rx_cntr = 0;
                }
                rx_buf[rx_cntr] = Serial2.read();
                rx_cntr++;
            }
            break;

        default:
            break;
        }
    }
    return rx_cntr;
}

int voice_ctrl::process(void)
{
    uint8_t idx;
    int status=0;
    uint8_t parse_started=false;

    if( rx_cntr )
    {
        for( idx=0; idx<RX_BUFFER_SIZE; idx++ )
        {
            if( rx_buf[ ( idx + VOICE_CTRL_BYTE_IDX_AA ) ] == VOICE_CTRL_BYTE_CODE_AA )
            {
                parse_started = true;
                if( (idx + MAX_PKT_SIZE) < RX_BUFFER_SIZE )
                {
                    if( rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_55) ] == VOICE_CTRL_BYTE_CODE_55 )
                    {
                        if( (rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_A5) ] == VOICE_CTRL_BYTE_CODE_A5) && (rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_5A) ] == VOICE_CTRL_BYTE_CODE_5A ))
                        {
                            if( (rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_CR) ] == VOICE_CTRL_BYTE_CODE_CR) && (rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_LF) ] == VOICE_CTRL_BYTE_CODE_LF ))
                            {
                                parse_started = false;
                                if( (rx_buf[ VOICE_CTRL_BYTE_IDX_CC ] == VOICE_CTRL_CMD_NONE ) || (rx_buf[ VOICE_CTRL_BYTE_IDX_CC ] >= VOICE_CTRL_CMD_ERR ))
                                {
                                    status = -1;
                                    data->err = VOICE_CTRL_ERR_CMD;
                                }
                                else
                                {
                                    data->cmd = rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_CC) ];
                                    data->value = rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_D1) ];
                                    data->value <<= 8;
                                    data->value |= rx_buf[ (idx + VOICE_CTRL_BYTE_IDX_D2) ];
                                    rx_cntr = 0;
                                    idx = RX_BUFFER_SIZE;
                                    for(int cntr=0;cntr<RX_BUFFER_SIZE;cntr++)
                                    {
                                        rx_buf[cntr] = 0;
                                    }
                                    if( cfg->cb_on_cmd_rx != nullptr )
                                    {
                                        cfg->cb_on_cmd_rx( data->cmd, data->value );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if( parse_started == true )
    {
        status = -1;
        data->err = VOICE_CTRL_ERR_PKT_FORMAT;
    }

    if( status != 0 )
    {
        if( cfg->cb_on_err != nullptr )
        {
            cfg->cb_on_err( data->err );
        }
    }

    return status;
}

void voice_ctrl::print_cfg(void)
{
    Serial.printf("vc cfg port:%d cb_on_cmd_rx:%p, cb_on_err:%p\n", cfg->port, cfg->cb_on_cmd_rx, cfg->cb_on_err);
}

void voice_ctrl::print_data(void)
{
    Serial.printf("vc data err:%d, cmd:%d value:%d\n", data->err, data->cmd, data->value);
}

void voice_ctrl::check(void)
{
    read();
    process();
}

const char *voice_ctrl::get_cmd(void)
{
    if( data->cmd < VOICE_CTRL_CMD_ERR )
    {
        return vc_cmds[data->cmd - 1];
    }
    
    return "ERR";
}

const char *voice_ctrl::get_err(void)
{
    if( data->err < VOICE_CTRL_ERR_MAX )
    {
        return vc_errs[data->err - 1];
    }
    
    return "NULL";
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_VOICE_CTRL)
voice_ctrl vc;
#endif

