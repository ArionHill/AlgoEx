#coding:utf-8
import sys
import datetime
import os
import urllib2
import jieba
import jieba.posseg as pseg
import subprocess
from bs4 import BeautifulSoup
import re
from pinyin import PinYin
import pdb
import time
import MySQLdb
import binascii
from struct import unpack

default_encoding = 'utf-8'
if sys.getdefaultencoding() != default_encoding:
    reload(sys)
    sys.setdefaultencoding(default_encoding)
try:
    import scapy.all as scapy
except ImportError:
    import scapy
try:
    # This import works from the project directory
    import scapy_http.http
except ImportError:
    # If you installed this package via pip, you just need to execute this
    from scapy.layers import http

def get_timeStample(filename):
    f = open(filename,'rb')
    string_data=f.read()
    #定义一个文件头的通用格式
    pcap_header = ['gmt_time','micro_time','pcap_content_len','len']
    #跳过文件头
    index=24;
    dict_all = {}
    count = 1
    while(index<len(string_data)):
        gmt_time=0
        pcap={}
        if len(string_data[index:16+index]) != 16:
            pass
        else:
            head=unpack("IIII",string_data[index:16+index])
            index +=16
            #得到数据包头
            head_dict=dict(zip(pcap_header,head))
            pcap_content_len=head_dict.get("pcap_content_len")
            gmt_time=head_dict.get("gmt_time")
            data_content=string_data[index:index+pcap_content_len]
            pcap=parse_iphdr(data_content)
            #print pcap
            if pcap!=None:
                time=datetime.datetime.fromtimestamp(gmt_time)

                pcap['time']=gmt_time
                #print time
                #print gmt_time
        dict_all[count] = pcap
        index += pcap_content_len
        count += 1
    return dict_all

def get_httpInformation(data):                                                                                          ###获取HTTP协议字段信息
    http_information = {}
    list = str(data).split()
    if len(list) > 0:
        pass
    else:
        return None
    if list[0] == 'POST':
        headers = str(data).split('\r\n')
        http_information['method'] = headers[0]
        for h in headers:
            if 'Host' in h:
                http_information['Host'] = h
            if 'User-Agent' in h:
                http_information['User_Agent'] = h
    elif list[0] == 'GET':
        headers = str(data).split('\r\n')
        http_information['method'] = headers[0]
        for h in headers:
            if 'Host' in h:
                http_information['Host'] = h
            if 'User-Agent' in h:
                http_information['User_Agent'] = h
    return http_information

def excute(pcap_path,result_filePath):

    for rt,dirs,files in os.walk(pcap_path):
        for d in dirs:
            data_path = rt + '/' + d + '/test.pcap'
            vector_path = rt + '/' + d + '/' + d +'_vector_test10-23.csv'
            print data_path
            apk_path = rt + '/' + d + '/' + d + '.apk'
            try:
                pcap = scapy.rdpcap(data_path)
            except Exception as e:
                print e
                continue
            dict_tcp = {}
            count = 1
            count_else = 0
            dict = get_timeStample(data_path)
            for p in pcap:
                traffic = {}
                proto = ''
                for f in p.payload.fields_desc:
                    if f.name == 'proto':
                        ct = scapy.conf.color_theme
                        vcol = ct.field_value
                        fvalue = p.payload.getfieldval(f.name)
                        # print f.i2repr(p.payload, fvalue)
                        proto = f.i2repr(p.payload, fvalue)
                        #print proto
                if proto == 'tcp':
                    pass
                else:
                    count_else += 1
                if proto == 'tcp':
                    for f in p.payload.fields_desc:
                        # print traffic['ip_src']
                        traffic['num'] = count
                        # print binascii.b2a_hex(str(p))[0:28]
                        str_hex = binascii.b2a_hex(str(p))
                        # print str_hex
                        # print str_hex[28:30]

                        if f.name == 'ihl':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            #print vcol
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_ihl'] = str(int(fvalue) * 4)
                            #print traffic['ip_ihl']
                            #print str_hex[28 + int(fvalue) * 8 : 28 + int(fvalue) * 8 + 2]
                        if f.name == 'src':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_src'] = f.i2repr(p.payload, fvalue)
                            #print traffic['ip_src']
                        if f.name == 'id':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_id'] = f.i2repr(p.payload, fvalue)
                            #print traffic['ip_src']
                        if f.name == 'dst':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_dst'] = f.i2repr(p.payload, fvalue)
                            #print traffic['ip_dst']
                        if f.name == 'tos':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_tos'] = f.i2repr(p.payload, fvalue)
                            # print traffic['ip_dst']
                        if f.name == 'len':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['ip_len'] = f.i2repr(p.payload, fvalue)
                            # print traffic['ip_dst']

                    for f in p.payload.payload.fields_desc:
                        if f.name == 'seq':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            #print fvalue
                            traffic['tcp_seq'] = f.i2repr(p.payload, fvalue)
                            #print traffic['tcp_seq']
                        if f.name == 'ack':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            #print fvalue
                            traffic['tcp_ack'] = f.i2repr(p.payload, fvalue)
                            #print traffic['tcp_ack']
                        if f.name == 'window':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            #print fvalue
                            traffic['tcp_window'] = f.i2repr(p.payload, fvalue)
                            #print traffic['tcp_ack']
                        if f.name == 'options':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            #print fvalue
                            traffic['tcp_options'] = f.i2repr(p.payload, fvalue)
                            #print traffic['tcp_ack']
                        if f.name == 'flags':
                            flag = ''
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            flags_value = str_hex[28 + int(traffic['ip_ihl']) * 2 + 26: 28 + int(traffic['ip_ihl']) * 2 + 28]
                            #print flags_value
                            if flags_value[0] == '8':
                                flag = 'C'
                            elif flags_value[0] == '4':
                                flag = 'E'
                            elif flags_value[0] == '2':
                                flag = 'U'
                            elif flags_value[0] == '1':
                                flag = 'A'
                            elif flags_value[0] == '0':
                                pass
                            if flags_value[1] == '8':
                                flag += 'P'
                            elif flags_value[1] == '4':
                                flag += 'R'
                            elif flags_value[1] == '2':
                                flag += 'S'
                            elif flags_value[1] == '1':
                                flag += 'F'
                            traffic['tcp_flags'] = flag
                        if f.name == 'sport':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['tcp_sport'] = f.i2repr(p.payload, fvalue)
                        if f.name == 'dport':
                            ct = scapy.conf.color_theme
                            vcol = ct.field_value
                            fvalue = p.payload.getfieldval(f.name)
                            traffic['tcp_dport'] = f.i2repr(p.payload, fvalue)
                        # if p.payload.payload.payload.name == 'HTTP':
                        #     traffic['data'] =
                        traffic['http_host'] = ''
                        if 'HTTP' in p:
                            if 'GET' or 'POST' in p:
                                if p.payload.payload.payload.name == 'HTTP':
                                    data = p.payload.payload.payload
                                    http_information = get_httpInformation(data)
                                    if http_information != None:
                                        if http_information.has_key('Host'):
                                            traffic['http_host'] = http_information['Host']
                    dict_tcp[count] = traffic

                else:
                    count_else += 1
                count += 1
            list_follow = get_tcpFellow(dict_tcp)
            if list_follow == None:
                continue
            for lf in list_follow:
                print lf
                print len(lf)
                for ll in lf:
                    dict_tcp[ll]['time_stample'] = dict[ll]['time']
                    print dict_tcp[ll]
            print count_else
            write_vectorFile(vector_path,dict_tcp,list_follow)

    return dict_tcp

def write_vectorFile(vector_path,dict_tcp,list_follow):
    with open(vector_path, 'w+') as f:
        content = 'Timestamp,Num,ip_srcAddress,ip_dstAddress,ip_ihl,ip_tos,ip_id,ip_len,tcp_srcPort,tcp_dstPort,tcp_seq,tcp_ack,tcp_flag,tcp_window,tcp_options,payload\r\n'
        f.write(content)
        for lf in list_follow:
            queue_len = ''
            queue_window = ''
            queue_options = ''
            queue_payload = ''
            for ll in lf:
                content = str(dict_tcp[ll]['time_stample']) + ',' + str(dict_tcp[ll]['num']) + ',' + dict_tcp[ll]['ip_src'] + ',' + dict_tcp[ll]['ip_dst'] + ',' + dict_tcp[ll]['ip_ihl'] + ',' + dict_tcp[ll]['ip_tos'] + ',' + dict_tcp[ll]['ip_id'] + ',' + dict_tcp[ll]['ip_len'] + ',' + dict_tcp[ll]['tcp_sport'] + ',' + dict_tcp[ll]['tcp_dport'] + ',' + dict_tcp[ll]['tcp_seq'] + ',' + dict_tcp[ll]['tcp_ack'] + ',' + dict_tcp[ll]['tcp_flags'] + ',' + dict_tcp[ll]['tcp_window'] + ',' + dict_tcp[ll]['tcp_options'] + ',' + dict_tcp[ll]['http_host'] + '\r\n'
                f.write(content)
                #print dict_tcp[ll]
                queue_len += dict_tcp[ll]['ip_len']
                queue_len += ','

                queue_window += dict_tcp[ll]['tcp_window']
                queue_window += ','

                queue_options += dict_tcp[ll]['tcp_options']
                queue_options += ','

                queue_payload += dict_tcp[ll]['http_host']
                queue_payload += ','
            queue_len += '\r\n'
            queue_window += '\r\n'
            queue_options += '\r\n'
            queue_payload += '\r\n'

            f.write(queue_len)
            f.write(queue_window)
            f.write(queue_options)
            f.write(queue_payload)

            content = '\r\n'
            f.write(content)


def parse_iphdr(data_content):
    index=0
    #分析以太网报头
    ip={}
    try:
        string_ethhdr=data_content[index:index+14]
        ethhdr=unpack("!BBBBBBBBBBBBH",string_ethhdr)

        index +=14
        #分析ip部分报头
        string_iphdr=data_content[index:index+20]
        iphdr=unpack('!BBHHHBBHBBBBBBBB',string_iphdr)
        #print iphdr
        ip["ip_src"]=`iphdr[8]`+':'+`iphdr[9]`+':'+`iphdr[10]`+':'+`iphdr[11]`;
        ip["ip_dst"]=`iphdr[12]`+':'+`iphdr[13]`+':'+`iphdr[14]`+':'+`iphdr[15]`;

        #获得协议类型，只分析tcp协议
        pro_type=iphdr[6]

        if pro_type==6:
            #分析tcp部分报头
            index+=20
            string_tcphdr=data_content[index:index+20]
            tcphdr=unpack('!HHIIBBHHH',string_tcphdr)
            #print tcphdr
            ip['port_src']=tcphdr[0]
            ip['port_dst']=tcphdr[1]
            #tcp头部的长度
            tcp_len=(tcphdr[4]//16)*4
            #移动到tcp报头的末尾
            index += tcp_len

            #http报文

            #print data_content[index:]
            #print "---------------------------------------"
            return ip
    except Exception as e :
        print e
        print len(string_ethhdr)
        return None

def get_tcpFellow(dict_tcp):
    count = 0
    list_follow = []
    index_list = dict_tcp.keys()
    try:
        for i in index_list:
            flag = dict_tcp[i]['tcp_flags']
            ack = dict_tcp[i]['tcp_ack']
            if flag == 'S' and ack == '0':                                                                                                  ###启动一个TCP会话,SYN
                #print '启动一个TCP会话,SYN'
                flag = False
                for j in index_list[index_list.index(i)+1:len(index_list)]:
                    dst = dict_tcp[i]['ip_dst']
                    src = dict_tcp[j]['ip_src']
                    if src == dst:
                        if dict_tcp[j]['ip_dst'] == dict_tcp[i]['ip_src']:
                            pass
                        else:
                            continue
                        if dict_tcp[j]['tcp_flags'] == 'AS' and dict_tcp[j]['tcp_ack'] == str(int(dict_tcp[i]['tcp_seq']) + 1):             ###第二次握手SYN和ACK
                            for k in index_list[index_list.index(j)+1:len(index_list)]:
                                if dict_tcp[k]['ip_src'] == dict_tcp[i]['ip_src'] and dict_tcp[k]['ip_dst'] == dict_tcp[i]['ip_dst']:
                                    if dict_tcp[k]['tcp_flags'] == 'A'and dict_tcp[k]['tcp_seq'] == dict_tcp[j]['tcp_ack']:                 ###第三次握手ACK
                                        flag = True
                                        address_A = dict_tcp[i]['ip_src']
                                        address_B = dict_tcp[i]['ip_dst']
                                        port_A = dict_tcp[i]['tcp_sport']
                                        port_B = dict_tcp[i]['tcp_dport']
                                        list = []
                                        list.append(i)
                                        list.append(j)
                                        list.append(k)
                                        break
                                    else:
                                        continue
                                else:
                                    continue
                        if flag == True:
                            break
                if flag == True:
                    if list != []:
                        #for h in range(list[2]+1,len(dict_tcp)):
                        for h in index_list[index_list.index(k) + 1:len(index_list)]:
                            if dict_tcp[h]['ip_src'] == address_A and dict_tcp[h]['ip_dst'] == address_B:
                                if dict_tcp[h]['tcp_sport'] == port_A and dict_tcp[h]['tcp_dport'] == port_B:
                                    list.append(h)
                            elif dict_tcp[h]['ip_src'] == address_B and dict_tcp[h]['ip_dst'] == address_A:
                                if dict_tcp[h]['tcp_sport'] == port_B and dict_tcp[h]['tcp_dport'] == port_A:
                                    list.append(h)
                        #print list
                        #print len(list)
                        list_follow.append(list)
                        count += 1
                    else:
                        continue
        print count
        return list_follow
    except Exception as e:
        print e
        return None

if __name__ == '__main__':
    pcap_path = '/home/cy/桌面/测试APK_10-9手工触发/no_action_file'
    result_filePath = 'asd'
    excute(pcap_path,result_filePath)


    # packets = scapy.rdpcap(pcap_path + '/好车轰轰/train.pcap')
    # for p in packets:
    #     p.show()
    #     pdb.set_trace()