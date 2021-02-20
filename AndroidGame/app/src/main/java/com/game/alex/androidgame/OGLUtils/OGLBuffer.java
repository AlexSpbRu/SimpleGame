package com.game.alex.androidgame.OGLUtils;



public class OGLBuffer {

    public class BufferExeption extends Exception {
        BufferExeption( String Text ){}
    }
    //
    public int		        m_iBuffSize = 0;
    public int              m_iDataSize = 0;
    public float			m_fData[];
    public short			m_iData[];
    public int				m_iSize = 0;
    //
    public OGLBuffer( int BuffSize, int  DataSize, Integer I ) {
        m_iDataSize = DataSize;
        m_iBuffSize = BuffSize * DataSize;
        m_iData = new short[m_iBuffSize];
    };
    public OGLBuffer( int BuffSize, int  DataSize, Float F ) {
        m_iDataSize = DataSize;
        m_iBuffSize = BuffSize * DataSize;
        m_fData = new float[m_iBuffSize];
    };
    //
    public int  Add( float[] Data, int Size ) throws BufferExeption {
        if( m_iSize + Size > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        for( int i = 0 ; i < Size ; i++ )
            m_fData[i + m_iSize] = Data[i];
        m_iSize += Size;
        return m_iSize / m_iDataSize;
    };
    //
    public int  Add( float D1, float D2 )  throws BufferExeption {
        if(m_iDataSize != 2 )
            throw new BufferExeption("Wrong size");
        if( m_iSize + 2 > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        m_fData[m_iSize] = D1;
        m_fData[m_iSize+1] = D2;
        m_iSize += 2;
        return m_iSize / m_iDataSize;
    };
    //
    public int  Add( float D1, float D2, float D3 )  throws BufferExeption {
        if(m_iDataSize != 3 )
            throw new BufferExeption("Wrong size");
        if( m_iSize + 3 > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        m_fData[m_iSize] = D1;
        m_fData[m_iSize+1] = D2;
        m_fData[m_iSize+2] = D3;
        m_iSize += 3;
        return m_iSize / m_iDataSize;
    };
    //
    public int  Add( float D1, float D2, float D3, float D4 )  throws BufferExeption {
        if(m_iDataSize != 4 )
            throw new BufferExeption("Wrong size");
        if( m_iSize + 4 > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        m_fData[m_iSize] = D1;
        m_fData[m_iSize+1] = D2;
        m_fData[m_iSize+2] = D3;
        m_fData[m_iSize+3] = D4;
        m_iSize += 4;
        return m_iSize / m_iDataSize;
    };
    //
    public int  Add( short[] Data, int Size )  throws BufferExeption {
        if( m_iSize + Size > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        for( int i = 0 ; i < Size ; i++ )
            m_iData[i + m_iSize] = Data[i];
        m_iSize += Size;
        return m_iSize / m_iDataSize;
    };
    //
    public int AddQuadIndex( short I1,  short I2,  short I3,  short I4,  short I5,  short I6 )  throws BufferExeption {
        if( m_iSize + 6 > m_iBuffSize)
            throw new BufferExeption("Buffer overflow");
        m_iData[ m_iSize] = I1;
        m_iData[1 + m_iSize] = I2;
        m_iData[2 + m_iSize] = I3;
        m_iData[3 + m_iSize] = I4;
        m_iData[4 + m_iSize] = I5;
        m_iData[5 + m_iSize] = I6;
        m_iSize += 6;
        return m_iSize / m_iDataSize;
    }
    //
    public int	Size()   { return m_iSize; };
    public int	Count()   { return m_iSize / m_iDataSize; };
    public float[]	GetData()   { return m_fData; };
    public short[]	GetDataI()   { return m_iData; };
    public void		Clear() { m_iSize = 0; };
}
