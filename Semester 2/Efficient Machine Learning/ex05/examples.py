import math

## 1.

def forward1( i_x,
              i_y,
              i_z ):
    l_a = i_y + i_z
    l_b = i_x * l_a

    return l_b

def backward1( i_x,
               i_y,
               i_z ):
    l_a = i_y + i_z
    
    l_dbda = i_x
    l_dbdx = l_a

    l_dady = 1
    l_dadz = 1

    l_dbdy = l_dbda * l_dady
    l_dbdz = l_dbda * l_dadz

    return l_dbdx, l_dbdy, l_dbdz


## 2.

def forward2( w0,
              w1,
              w2,
              x0,
              x1 ):
    mul0 = w0 * x0
    mul1 = w1 * x1
    add01 = mul0 + mul1
    add2 = add01 + w2
    neg = -add2
    exp = math.exp(neg)
    plus1 = 1 + exp
    rec = 1 / plus1

    return rec

def backward2( w0,
               w1,
               w2,
               x0,
               x1 ):
    mul0 = w0 * x0
    mul1 = w1 * x1
    add01 = mul0 + mul1
    add2 = add01 + w2
    neg = -add2
    exp = math.exp(neg)
    plus1 = 1 + exp

    # all one-step derivatives
    drec_dplus1 = -(1.0 / (plus1*plus1))
    dplus1_dexp = 1.0
    dexp_dneg = exp
    dneg_dadd2 = -1.0
    dadd2_dadd01 = 1.0
    dadd2_dw2 = 1.0
    dadd01_dmul0 = 1.0
    dadd01_dmul1 = 1.0
    dmul0_dw0 = x0
    dmul0_dx0 = w0
    dmul1_dw1 = x1
    dmul1_dx1 = w1

    # chain rule applications
    drec_dexp = drec_dplus1 * dplus1_dexp
    drec_dneg = drec_dexp * dexp_dneg
    drec_dadd2 = drec_dneg * dneg_dadd2
    drec_dadd01 = drec_dadd2 * dadd2_dadd01
    drec_dw2 = drec_dadd2 * dadd2_dw2
    drec_dmul0 = drec_dadd01 * dadd01_dmul0
    drec_dmul1 = drec_dadd01 * dadd01_dmul1
    drec_dw0 = drec_dmul0 * dmul0_dw0
    drec_dx0 = drec_dmul0 * dmul0_dx0
    drec_dw1 = drec_dmul1 * dmul1_dw1
    drec_dx1 = drec_dmul1 * dmul1_dx1

    return drec_dw0, drec_dw1, drec_dw2, drec_dx0, drec_dx1

def forward3 ( x, y ):
    prod = x * y
    sum1 = x + y
    diff = x - y
    sin = math.sin(prod)
    cos = math.cos(sum1)
    exp = math.exp(diff)
    sum2 = sin + cos
    rec = sum2 / exp

    return rec

def backward3 ( x, y ):
    prod = x * y
    sum1 = x + y
    diff = x - y
    sin = math.sin(prod)
    cos = math.cos(sum1)
    exp = math.exp(diff)
    sum2 = sin + cos

    # all one-step derivatives
    drec_dsum2 = 1.0 / (exp)
    drec_dexp = - sum2 / (exp*exp)
    dsum2_dsin = 1.0
    dsum2_dcos = 1.0
    dsin_dprod = math.cos(prod)
    dcos_dsum1 = -math.sin(sum1)
    dprod_dx = y
    dprod_dy = x
    dsum1_dx = 1.0
    dsum1_dy = 1.0
    dexp_ddiff = exp
    ddiff_dx = 1.0
    ddiff_dy = -1.0

    # chain rule applications
    drec_dsin = drec_dsum2 * dsum2_dsin
    drec_dcos = drec_dsum2 * dsum2_dcos
    drec_dprod = drec_dsin * dsin_dprod
    drec_dsum1 = drec_dcos * dcos_dsum1
    drec_ddiff = drec_dexp * dexp_ddiff
    drec_dx = ( drec_dprod * dprod_dx
              + drec_dsum1 * dsum1_dx
              + drec_ddiff * ddiff_dx )
    drec_dy = ( drec_dprod * dprod_dy
              + drec_dsum1 * dsum1_dy
              + drec_ddiff * ddiff_dy )
    return drec_dx, drec_dy
     