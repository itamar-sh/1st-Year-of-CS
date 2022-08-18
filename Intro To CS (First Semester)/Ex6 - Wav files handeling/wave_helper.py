import wave
import struct
import sys

def load_wave(wave_filename):
    try :
        fin = wave.open(wave_filename,'r')
        num_frames = fin.getnframes()
        data = []
        while (fin.tell() < num_frames):
            frame = fin.readframes(1)
            data_p = []
            if fin.getsampwidth() == 1:
                data_p.append(struct.unpack('%dB'%(fin.getnchannels()), frame))
            elif fin.getsampwidth() == 2:
                data_p.append(struct.unpack('%dh'%(fin.getnchannels()), frame))
            else :
                fin.close()
                raise Exception('Unhandeled sample width')
            if fin.getnchannels() == 1 :
                data_p[0] = (data_p[0][0],data_p[0][0])
            elif fin.getnchannels() > 2 :
                data_p[0] = (data_p[0][0],data_p[0][1])
            if fin.getsampwidth() == 1 :
                data_p[0] = (256*(data_p[0][0]-128), 256*(data_p[0][1]-128))
            data.append(list(data_p[0]))
        fin.close()
        return fin.getframerate(), \
               data
    except KeyboardInterrupt:
        raise
    except :
        return -1

def save_wave(frame_rate, audio_data, wave_filename):
    try :
        fout = wave.open(wave_filename,'w')
        fout.setparams((2,
                        2,
                        frame_rate,
                        0,
                        'NONE',
                        'not compressed'))
        values = []
        for frame in audio_data:
            for data in frame:
                values.append(struct.pack('h', data))
        value_str = b"".join(values)
        fout.writeframes(value_str)
        fout.close()
        return 0
    except KeyboardInterrupt:
        raise
    except :
        return -1

