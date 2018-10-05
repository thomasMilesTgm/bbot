import serial
import termios
import tty, sys
import curses
import string

BUAD = 250000
PORT = '/dev/ttyUSB0'

neutral = 0
boost_max_l = 100
boost_min_l = -100
boost_max_r = 100
boost_min_r = -100
normal_max_l = 30
normal_min_l = -30
normal_max_r = 30
normal_min_r = -30
acc_left = 1        # % per increment
acc_right = 1        # % per increment

boost_acc = 10

m_left = ' motor_left,'
m_right = ' motor_right,'
breaks = ' stop,1~'
l_calib = ' calibrate_left,1~'
r_calib = ' calibrate_right,1~'
end = '~'

fwd = 'w'
back = 's'
left = 'a'
right = 'd'

brk = 'b'
boost = ' '

# Trims for adjusting motor speed limits and accelerations
trim_left = 'j'
trim_right = 'l'
trim_acc_left_up = 'u'
trim_acc_left_down = 'n'
trim_acc_right_up = 'o'
trim_acc_right_down = '.'
trim_rate = 1

def map_curses(curse):
    if curse == 119:
        return fwd
    elif curse == 97:
        return left
    elif curse == 100:
        return right
    elif curse == 115:
        return back
    elif curse == 98:
        return brk
    elif curse == 32:
        return boost
    elif curse == 106:
        return trim_left
    elif curse == 108:
        return trim_right
    elif curse == 117:
        return trim_acc_left_up
    elif curse == 110:
        return trim_acc_left_down
    elif curse == 111:
        return trim_acc_right_up
    elif curse == 46:
        return trim_acc_right_down

def encode_to_bytes(string):
    b = string.encode('utf-8')
    return b

if __name__ == '__main__':
    k = curses.initscr()
    # curses.cbreak()
    curses.noecho()
    k.keypad(1)
    k.nodelay(True)

    v_left = 0
    v_right = 0
    boosting = False

    ard = serial.Serial(PORT, BUAD, timeout=5)
    # start in neutral
    ard.write(encode_to_bytes(m_left+str(neutral)))
    ard.write(encode_to_bytes(m_right + str(neutral)))

    old = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    while True:


        try:



            k.addstr(0, 0, str(ard.readline())+"\n", curses.A_BLINK)
            k.refresh()





            key = map_curses(k.getch())

            while not (key == fwd or key == back or key == left or key == right or
                       key == brk or key == boost or
                       key == trim_left or key == trim_right or
                       key == trim_acc_right_down or key == trim_acc_right_up or
                       key == trim_acc_left_up or key == trim_acc_left_down):
                key = map_curses(k.getch())
           #
            if key == fwd:

                if v_left < normal_max_l:
                    v_left += acc_left
                    ard.write(encode_to_bytes(m_left + str(v_left) + end))

                if v_right < normal_max_r:
                    v_right += acc_right
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                else:
                    ard.write(encode_to_bytes(m_right + str(normal_max_r) + end))
                    ard.write(encode_to_bytes(m_left + str(normal_max_l) + end))

            elif key == back:

                if v_left > normal_min_l:
                    v_left -= acc_left
                    ard.write(encode_to_bytes(m_left + str(v_left) + end))

                if v_right > normal_min_r:
                    v_right -= acc_right
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                else:
                    ard.write(encode_to_bytes(m_right + str(normal_min_r) + end))
                    ard.write(encode_to_bytes(m_left + str(normal_min_l) + end))

            elif key == left:

                if v_left > normal_min_l:
                    v_left -= acc_left
                    ard.write(encode_to_bytes(m_left + str(v_left) + end))

                else:
                    ard.write(encode_to_bytes(m_left + str(normal_min_l) + end))

                if v_right < normal_max_r:
                    v_right += acc_right
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                else:
                    ard.write(encode_to_bytes(m_right + str(normal_max_r) + end),)


            elif key == right:

                if v_left < normal_max_l:
                    v_left += acc_left
                    ard.write(encode_to_bytes(m_left + str(v_left) + end))

                else:
                    ard.write(encode_to_bytes(m_left + str(normal_max_l) + end))

                if v_right > normal_min_r:
                    v_right -= acc_right
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                else:
                    ard.write(encode_to_bytes(m_right + str(normal_min_r) + end))


            elif key == brk:
                v_right=0
                v_left=0
                ard.write(encode_to_bytes(breaks))


            elif key == boost:

                if v_right > 0 and v_right < boost_max_r:
                    v_right += boost_acc
                    if v_right > boost_max_r:
                        v_right = boost_max_r
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                if v_left > 0 and v_left < boost_max_l:
                    v_left += boost_acc
                    if v_left > boost_max_l:
                        v_left = boost_max_l
                    ard.write(encode_to_bytes(m_right + str(v_left) + end))

                if v_right < 0 and v_right > boost_min_r:
                    v_right -= boost_acc
                    if v_right < boost_min_r:
                        v_right = boost_min_r
                    ard.write(encode_to_bytes(m_right + str(v_right) + end))

                if v_left < 0 and v_left > boost_min_l:
                    v_left -= boost_acc
                    if v_left < boost_min_l:
                        v_left = boost_min_l
                    ard.write(encode_to_bytes(m_right + str(v_left) + end))


            elif key == trim_left:

                normal_max_l -= trim_rate
                normal_min_l += trim_rate
                normal_max_r += trim_rate
                normal_min_r -= trim_rate


            elif key == trim_right:

                normal_max_l += trim_rate
                normal_min_l -= trim_rate
                normal_max_r -= trim_rate
                normal_min_r += trim_rate

            elif key == trim_acc_left_up:

                acc_left += trim_rate

            elif key == trim_acc_left_down:

                acc_left -= trim_rate

            elif key == trim_acc_right_up:

                acc_right += trim_rate

            elif key == trim_acc_right_down:

                acc_right -= trim_rate

            k.clear()

        except KeyboardInterrupt:
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old)
            break

        finally:
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old)

