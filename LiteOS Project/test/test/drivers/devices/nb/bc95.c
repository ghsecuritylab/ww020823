/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include <string.h>
#include <ctype.h>
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "bc95.h"

#define NB_STAT_LOCALPORT 56
#define AT_LINE_END 		"\r\n"
#define AT_CMD_BEGIN		"\r\n"
#define AT_DATAF_PREFIX      "+NSONMI:"
#define MAX_SOCK_NUM 5
#define UDP_PROTO   17
#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif /* MIN */

extern at_task at;
at_adaptor_api at_interface;
extern char rbuf[AT_DATA_LEN];
extern char wbuf[AT_DATA_LEN];
at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart_port = AT_USART_PORT,
    .buardrate = AT_BUARDRATE,
    .linkid_num = AT_MAX_LINK_NUM,
    .user_buf_len = MAX_AT_USERDATA_LEN,
    .cmd_begin = AT_CMD_BEGIN,
    .line_end = AT_LINE_END,
    .mux_mode = 1, //support multi connection mode
    .timeout = AT_CMD_TIMEOUT,   //  ms
};

char tmpbuf[AT_DATA_LEN]={0}; //transform to hex

socket_info sockinfo[MAX_SOCK_NUM];
int str_to_hex(const char *bufin, int len, char *bufout)
{
    int i = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i++)
    {
        sprintf(bufout+i*2, "%02X", bufin[i]);
    }
    return 0;
}

void HexStrToStr(const unsigned char *source, unsigned char *dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;
    for (i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(source[i]);
        lowByte  = toupper(source[i + 1]);
        if (highByte > 0x39)
            highByte -= 0x37;
        else
            highByte -= 0x30;
        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;
        dest[i / 2] = (highByte << 4) | lowByte;
    }
    return ;
}

int chartoint(char* port)
{
	int tmp=0;
	while(*port >= '0' && *port <= '9')
	{
		tmp = tmp*10+*port-'0';
		port++;
	}
	return tmp;
}

int32_t nb_reboot(void)
{
    memset(sockinfo, 0, MAX_SOCK_NUM * sizeof(struct _socket_info_t));
    return at.cmd((int8_t*)AT_NB_reboot, strlen(AT_NB_reboot), "OK", NULL,NULL);
}

int32_t nb_hw_detect(void)//"AT+CFUN?\r"
{
    return at.cmd((int8_t*)AT_NB_hw_detect, strlen(AT_NB_hw_detect), "+CFUN:1", NULL,NULL);
}

int32_t nb_check_csq(void)
{
    char *cmd = "AT+CSQ\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "+CSQ:", NULL,NULL);
}

int32_t nb_set_cdpserver(char* host, char* port)
{
    char *cmd = "AT+NCDP=";
    char *cmd2 = "AT+NCDP?";
	char *cmdNNMI = "AT+NNMI=1\r";
    char *cmdCMEE = "AT+CMEE=1\r";
	//char *cmdCGP = "AT+CGPADDR";
	char tmpbuf[128] = {0};
	int ret = -1;
    char ipaddr[100] = {0};
    if(strlen(host) > 70 || strlen(port) > 20 || (host==NULL && port == NULL))
    {
        ret = at.cmd((int8_t*)cmdNNMI, strlen(cmdNNMI), "OK", NULL,NULL);
        ret = at.cmd((int8_t*)cmdCMEE, strlen(cmdCMEE), "OK", NULL,NULL);
        return ret;
    }

    snprintf(ipaddr, sizeof(ipaddr) - 1, "%s,%s\r", host, port);

	snprintf(tmpbuf, sizeof(tmpbuf) - 1, "%s%s%c", cmd, ipaddr, '\r');
    ret = at.cmd((int8_t*)tmpbuf, strlen(tmpbuf), "OK", NULL,NULL);
	if(ret < 0)
	{
		return ret;
	}
	ret = at.cmd((int8_t*)cmd2, strlen(cmd2), ipaddr, NULL,NULL);
	LOS_TaskDelay(1000);
	ret = at.cmd((int8_t*)cmdNNMI, strlen(cmdNNMI), "OK", NULL,NULL);
	//ret = at.cmd((int8_t*)cmdCGP, strlen(cmdCGP), NULL, NULL,NULL);
	return ret;
}

int32_t nb_send_psk(char* pskid, char* psk)
{
    char* cmds = "AT+QSECSWT";//AT+QSECSWT=1,100    OK
    char* cmdp = "AT+QSETPSK";//AT+QSETPSK=86775942,E6F4C799   OK
    sprintf(wbuf, "%s=%d,%d\r", cmds, 1, 100);//min
    at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);
    snprintf(wbuf, AT_DATA_LEN, "%s=%s,%s\r", cmdp, pskid, psk);
    return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);
}

int32_t nb_set_no_encrypt(void)
{
    char* cmd = "AT+QSECSWT=0\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL,NULL);
}

int32_t nb_send_str(const char* buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret;
    char* str = NULL;
    int curcnt = 0;
    int rbuflen;
    static int sndcnt = 0;
    memset(wbuf, 0, AT_DATA_LEN);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN, "%s%d,%s%c",cmd1,(int)len/2,buf,'\r');
    ret = at.cmd((int8_t*)wbuf, strlen(wbuf), NULL, NULL,NULL);
    if(ret < 0)
        return -1;
    ret = at.cmd((int8_t*)cmd2, strlen(cmd2), "SENT=", rbuf,&rbuflen);
    if(ret < 0)
        return -1;
    str = strstr(rbuf,"SENT=");
    if(str == NULL)
        return -1;
    sscanf(str,"SENT=%d,%s",&curcnt,wbuf);
    if(curcnt == sndcnt)
        return -1;
    sndcnt = curcnt;
    return ret;
}

int32_t nb_send_payload(const char* buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret;
    char* str = NULL;
    int curcnt = 0;
    int rbuflen;
    static int sndcnt = 0;
    if(buf == NULL || len > AT_MAX_PAYLOADLEN)
    {
        AT_LOG("payload too long");
        return -1;
    }
    memset(tmpbuf, 0, AT_DATA_LEN);
    memset(wbuf, 0, AT_DATA_LEN);
    str_to_hex(buf, len, tmpbuf);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN,"%s%d,%s%c",cmd1,(int)len,tmpbuf,'\r');
    ret = at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);
    if(ret < 0)
        return -1;
    ret = at.cmd((int8_t*)cmd2, strlen(cmd2), "SENT=", rbuf,&rbuflen);
    if(ret < 0)
        return -1;
    str = strstr(rbuf,"SENT=");
    if(str == NULL)
        return -1;
    sscanf(str,"SENT=%d,%s",&curcnt,wbuf);
    if(curcnt == sndcnt)
        return -1;
    sndcnt = curcnt;
    return ret;
}

int nb_query_ip(void)
{
	char *cmd = "AT+CGPADDR\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "+CGPADDR:0,", NULL,NULL);
}

int32_t nb_get_netstat(void)
{
	char *cmd = "AT+CGATT?\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "CGATT:1", NULL,NULL);
}


int32_t nb_create_sock(int port,int proto)
{
	int socket;
    int rbuflen = AT_DATA_LEN;
	char *cmdudp = "AT+NSOCR=DGRAM,17,";//udp
	char *cmdtcp = "AT+NSOCR=STREAM,6,";//tcp
	int ret;
	if(proto!=17 && proto!=6)
    {
        AT_LOG("proto invalid!");
        return -1;
    }
    memset(rbuf, 0, AT_DATA_LEN);
	memset(wbuf, 0, AT_DATA_LEN);

    if(proto == 17)
        snprintf(wbuf, AT_DATA_LEN, "%s%d,1\r", cmdudp, port);//udp
    else
        {snprintf(wbuf, AT_DATA_LEN, "%s%d,1\r", cmdtcp, port);}
	at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", rbuf, &rbuflen);
	ret = sscanf(rbuf, "%d\r%s",&socket, tmpbuf);
	//neul_bc95_hex_to_str(tmpbuf, readlen*2, coapmsg);
    if ((2 == ret) && (socket >= 0)
        && (strnstr(tmpbuf, "OK", sizeof(tmpbuf))))
    {
        return socket;
    }

    AT_LOG("sscanf fail,ret=%d,socket=%d", ret, socket);
    return -1;
}

int nb_decompose_str(char* str,QUEUE_BUFF* qbuf, int* rlen,int* readleft)
{
    int port;
    unsigned char bufout[AT_DATA_LEN/2]={0};
    char *tmp,*trans;
    tmp = strstr(str,",");
    if(tmp == NULL)
        goto END;
    //chartoint((char*)tmp);
    trans = strstr(tmp+1,",");
    if(trans == NULL)
        goto END;
    strncpy(qbuf->ipaddr,tmp+1,MIN((trans-tmp),AT_DATA_LEN/2));
    qbuf->ipaddr[trans-tmp-1] = '\0';

    port = chartoint((char*)(trans+1));

    qbuf->port = port;
    tmp = strstr(trans+1,",");
    if(tmp == NULL)
        goto END;
    *rlen = chartoint((char*)(tmp+1));

    if(*rlen >= AT_DATA_LEN/2 || *rlen < 0)
        goto END;
    trans = strstr(tmp+1,",");
    if(trans == NULL)
        goto END;
    HexStrToStr((const unsigned char*)(trans+1), bufout, (*rlen)*2);
    if ((qbuf->len + *rlen) > AT_DATA_LEN/2)
    {
        AT_LOG("at rcv len exceed err len %ld, rcv len %d", qbuf->len, *rlen);
        goto END;
    }

    memcpy(qbuf->addr+qbuf->len, bufout, *rlen);
    qbuf->len += *rlen;
    tmp = strstr(trans+1,",");
    if(tmp == NULL)
        goto END;
    *readleft = chartoint((char*)(tmp+1));
    return qbuf->len;
END:
    {
        AT_LOG("decompose fail!");
        *rlen = 0;
        return -1;
    }
}

int32_t nb_data_rcv_handler(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }

    #define CMDBUF_LEN 40
    int32_t ret = -1;
    int32_t sockid = 0, data_len = 0;
    char *p1, *p2;
    int rbuflen = AT_DATA_LEN;
    QUEUE_BUFF qbuf;
    char cmdbuf[CMDBUF_LEN] = {0};
    int cmdlen;
    p1 = (char *)buf;
    char* cmd = "AT+NSORF=";
    int rlen,readleft = 0;
    char* str;

    char* rcvbuf = at_malloc(at_user_conf.user_buf_len);
    if (NULL == rcvbuf)
    {
        AT_LOG("malloc recvbuf failed!");
        goto END;
    }


    memset(&qbuf,0,sizeof(QUEUE_BUFF));
    p2 = strstr(p1, AT_DATAF_PREFIX);
    if (NULL == p2)
    {
        AT_LOG("got data prefix invailed!");
        goto END;
    }
    p2+=strlen(AT_DATAF_PREFIX);

    for (p2++; *p2 <= '9' && *p2 >= '0'; p2++)
    {
        sockid = sockid * 10 + (*p2 - '0');
    }

    for (p2++; *p2 <= '9' && *p2 >= '0' ;p2++)
    {
        data_len = (data_len * 10 + (*p2 - '0'));
    }

    qbuf.addr = at_malloc(AT_DATA_LEN/2);//extra space for ip and port
    if (NULL == qbuf.addr)
    {
        AT_LOG("malloc for qbuf failed!");
        goto END;
    }


    cmdlen = snprintf(cmdbuf, CMDBUF_LEN, "%s%d,%d\r", cmd, (int)sockid,(int)data_len);
    (void)at_cmd_in_recv_task((int8_t*)cmdbuf, cmdlen, "OK", rcvbuf,&rbuflen);

    if(rcvbuf!= NULL)
	{
	    (void) nb_decompose_str(rcvbuf,&qbuf,&rlen,&readleft);
        while(readleft != 0 && rcvbuf != NULL)
        {
            memset(cmdbuf, 0, CMDBUF_LEN);
            memset(rcvbuf, 0, MIN(at_user_conf.user_buf_len,rbuflen+1));
            rbuflen = AT_DATA_LEN;
            cmdlen = snprintf(cmdbuf, CMDBUF_LEN, "%s%d,%d\r", cmd, (int)sockid,(int)readleft);
            (void)at_cmd_in_recv_task((int8_t*)cmdbuf, cmdlen, "OK", rcvbuf,&rbuflen);

            str = strstr(rcvbuf, AT_DATAF_PREFIX);
            str+=strlen(AT_DATAF_PREFIX);
            if(str != NULL)
            {
                str = strstr(rcvbuf, ",");//todo
                str+=1;
            }
            (void)nb_decompose_str(str,&qbuf,&rlen,&readleft);
        }
	}
    //*(qbuf.addr+tolen) = NULL;

    if (LOS_OK != (ret = LOS_QueueWriteCopy(at.linkid[sockid].qid, &qbuf, sizeof(QUEUE_BUFF), 0)))
    {
        AT_LOG("LOS_QueueWriteCopy  failed!");
        at_free(qbuf.addr);
        goto END;
    }
    ret = data_len;
    END:
    if (NULL != rcvbuf)
    {
        at_free(rcvbuf);
        rcvbuf = NULL;
    }
    return ret;
}

int32_t nb_bind(const int8_t * host, const int8_t *port, int32_t proto)
{
	int ret = 0;
	int portnum;
	portnum = chartoint((char*)port);

    ret = nb_create_sock(portnum, UDP_PROTO);

	if(ret >= MAX_SOCK_NUM || ret < 0)
	{
		AT_LOG_DEBUG("sock num exceeded,ret is %d",ret);
		return -1;
	}
    memset(&sockinfo[ret], 0, sizeof(sockinfo[ret]));
	//memcpy(sockinfo[ret].localip, (const char*)host,strlen((const char*)host));
	sockinfo[ret].localport = *(unsigned short*)portnum;

    if (LOS_QueueCreate("dataQueue", 16, &at.linkid[ret].qid, 0, sizeof(QUEUE_BUFF)) != LOS_OK)
    {
        AT_LOG("init dataQueue failed, ret is %d!",ret);
        at.linkid[ret].usable = AT_LINK_UNUSE;//adapter other module.
        return -1;
    }

    return ret;

}

int32_t nb_connect(const int8_t * host, const int8_t *port, int32_t proto)
{
	int ret = 0;
    int portnum;
	static int localport = NB_STAT_LOCALPORT;

	do{
		ret = nb_create_sock(localport, UDP_PROTO);
		localport++;
	}while(ret < 0);

	localport--;
	if(ret >= MAX_SOCK_NUM ||ret < 0)
	{
		AT_LOG_DEBUG("sock num exceeded,ret is %d",ret);
		return -1;
	}
    portnum = chartoint((char*)port);
	memcpy(sockinfo[ret].remoteip, (const char*)host,strlen((const char*)host));
	sockinfo[ret].remoteport = portnum;
    AT_LOG("ret:%d remoteip:%s port:%d",ret,sockinfo[ret].remoteip,sockinfo[ret].remoteport);
	localport++;

    if (LOS_QueueCreate("dataQueue", 16, &at.linkid[ret].qid, 0, sizeof(QUEUE_BUFF)) != LOS_OK)
    {
        AT_LOG("init dataQueue failed, ret is %d!",ret);
        at.linkid[ret].usable = AT_LINK_UNUSE;//adapter other module.
        return -1;
    }

    return ret;

}

int32_t nb_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	char *cmd = "AT+NSOST=";
	//char *str = "AT+NMGS192.53.100.53,5683,1,11\r";
	int data_len = len/2;
    if(buf == NULL || data_len > AT_MAX_PAYLOADLEN)
    {
        AT_LOG("payload too long");
        return -1;
    }
    AT_LOG("id:%d remoteip:%s port:%d",(int)id,sockinfo[id].remoteip,sockinfo[id].remoteport);
	memset(wbuf, 0, AT_DATA_LEN);
	memset(tmpbuf, 0, AT_DATA_LEN);
	str_to_hex((const char *)buf, len, tmpbuf);
	snprintf(wbuf, AT_DATA_LEN, "%s%d,%s,%d,%d,%s\r",cmd,(int)id,sockinfo[id].remoteip,(int)sockinfo[id].remoteport,(int)len,tmpbuf);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);
}

int32_t nb_sendto(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port)
{
	char *cmd = "AT+NSOST=";
	int data_len = len/2;
    if(buf == NULL || data_len > AT_MAX_PAYLOADLEN)
    {
        AT_LOG("payload too long");
        return -1;
    }
	memset(wbuf, 0, AT_DATA_LEN);
	memset(tmpbuf, 0, AT_DATA_LEN);
	str_to_hex((const char *)buf, len, tmpbuf);
	snprintf(wbuf, AT_DATA_LEN, "%s%d,%s,%d,%d,%s\r",cmd,(int)id,ipaddr,*port,(int)data_len,tmpbuf);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);
}

int32_t nb_recv(int32_t id , uint8_t  *buf, uint32_t len)
{
    return nb_recv_timeout(id, buf, len,NULL,NULL, LOS_WAIT_FOREVER);
}

int32_t nb_recvfrom(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port)
{
    return nb_recv_timeout(id, buf, len, ipaddr,port,LOS_WAIT_FOREVER);
}

int32_t nb_recv_timeout(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port, int32_t timeout)
{
    int rlen = 0;
    int ret;
    QUEUE_BUFF	qbuf;
    UINT32 qlen = sizeof(QUEUE_BUFF);

    if (id  >= MAX_SOCK_NUM)
    {
        AT_LOG("link id %ld invalid", id);
        return -1;
    }

    memset(rbuf, 0, AT_DATA_LEN);
    memset(tmpbuf, 0, AT_DATA_LEN);

    ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout);
    if (ret != LOS_OK)
    {
        return -1;
    }

    if (('\0' == sockinfo[id].remoteip[0])
        || (0 == sockinfo[id].remoteport))
    {
        AT_LOG("update ip and port for link %ld", id);
        strncpy(sockinfo[id].remoteip, qbuf.ipaddr, sizeof(sockinfo[id].remoteip));
        sockinfo[id].remoteip[sizeof(sockinfo[id].remoteip) - 1] = '\0';
        sockinfo[id].remoteport = qbuf.port;
    }

	if(ipaddr != NULL)
	{
	    memcpy(ipaddr,qbuf.ipaddr,strlen(qbuf.ipaddr));
        *port = qbuf.port;
	}
    rlen = qbuf.len;

    if (rlen){
        memcpy(buf, qbuf.addr, rlen);
        at_free(qbuf.addr);
    }
    return rlen;

}

int32_t nb_close(int32_t socket)
{
    char *cmd = "AT+NSOCL=";
	memset(wbuf, 0, AT_DATA_LEN);
	sprintf(wbuf, "%s%d\r", cmd, (int)socket);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL,NULL);

}

int32_t nb_recv_cb(int32_t id)
{
    return -1;
}

int32_t nb_deinit(void)
{
    return nb_reboot();
}

at_adaptor_api at_interface = {
    .init = NULL,

    .bind = nb_bind,

    .connect = nb_connect,
    .send = nb_send,
    .sendto = nb_sendto,

    .recv_timeout = nb_recv_timeout,
    .recv = nb_recv,
    .recvfrom = nb_recvfrom,

    .close = nb_close,
    .recv_cb = nb_recv_cb,

    .deinit = nb_deinit,
};
#endif
