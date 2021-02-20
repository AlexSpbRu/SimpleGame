#pragma once

#include <assert.h>

class  CGLTexture;

template < int DataSize, int BuffSize , class DataType = float >
struct SOGLBuffer {
	constexpr static  size_t		m_iBuffSize = BuffSize * DataSize;
	using value_type = typename std::conditional<  DataSize == 1, std::tuple< DataType >,
		typename std::conditional<  DataSize == 2, std::tuple< DataType, DataType >,
			typename std::conditional< DataSize == 3, std::tuple< DataType, DataType, DataType >,
				typename std::conditional< DataSize == 4, std::tuple< DataType, DataType, DataType, DataType >, void >::type
			>::type
		>::type
	>::type;
	//
	std::vector< DataType >			m_vData;
	size_t							m_iSize = 0;
	//
	SOGLBuffer() {
		m_vData.resize(m_iBuffSize);
	};

	//-------------------------------------------------------
	//   этот метод для вызова с числом аргументов = DataSize 
	// этот метод не вызывается из Add(std::forward<Float>(X) ...);
	
	template< int Num, class Tuple >
	struct SAddTuple : public SAddTuple<Num-1, Tuple> {
		SAddTuple( Tuple& tup, std::vector< DataType >& Data, size_t& Size) : SAddTuple<Num - 1, Tuple>(tup, Data, Size) {
			assert( Size < BuffSize);
			Data[Size] = std::get<Num - 1>(tup);
			Size++;
		};
	};
	template< class Tuple >
	struct SAddTuple< 0, Tuple > {
		SAddTuple( Tuple& tup, std::vector< DataType >& Data, size_t& Size ) {
		}
	};
//  если вызвать Add(X ...) - то ошибка value_type  = tuple< DataType, DataType > , а  tup = tuple< DataType, ... >
	template< class... T >
	size_t	Add(T ... X) {
		value_type  tup = std::tie(X ...);
		using arg_type = decltype(std::tie(X ...));
		SAddTuple<DataSize, value_type>(tup,m_vData, m_iSize);

		return m_iSize / DataSize;
	};

	//-------------------------------------------------------
	//  Вместо функции с переменным числом ааргументов используем передачу vector по ссылке &&

	inline size_t  Add( std::vector< DataType >&& Data ) {
		auto sz = Data.size();
		assert(m_iSize + sz < BuffSize);
		memcpy(m_vData.data() + m_iSize, &Data[0], sz * sizeof(DataType));
		m_iSize += sz;
		return m_iSize / DataSize;
	}
	/*size_t	Add( std::initializer_list<GLfloat> Data ){
		assert()
	};*/

	inline size_t  Add(const DataType* Begin, const DataType* End) {
		auto sz = End - Begin;
		assert( m_iSize + sz < BuffSize);
		memcpy(m_vData.data() + m_iSize, Begin, sz*sizeof(DataType));
		m_iSize += sz;
		return m_iSize / DataSize;
	};
	inline size_t  Add(DataType* Begin, DataType* End) {
		auto sz = End - Begin;
		assert(m_iSize + sz < BuffSize);
		memcpy(m_vData.data() + m_iSize, Begin, sz * sizeof(DataType));
		m_iSize += sz;
		return m_iSize / DataSize;
	};
	//
	inline size_t	Size() const noexcept { return m_iSize; };
	inline size_t	Count() const noexcept { return m_iSize / DataSize; };
	inline const DataType*	GetData() const noexcept { return m_vData.data(); };
	inline void		Clear() { m_iSize = 0; };
};


template < int VertexDim, int ColorDim,  int BuffSize = 2048 >
class COGLDrawBuffer {
	SOGLBuffer< VertexDim, BuffSize >		m_vVertex;
	SOGLBuffer< 2, BuffSize >				m_vTexture;
	SOGLBuffer< ColorDim, BuffSize >		m_vColor;
	SOGLBuffer< 1, BuffSize*2, unsigned int >		m_vIndex;
	GLuint									m_iVBO = -1;
	GLuint									m_iTBO = -1; 
	GLuint									m_iCBO = -1;
#ifdef OPENGL_SHADER
	SOGLMatrix								m_ModelMatrix;
	Shader*									m_Shader = nullptr;
	GLuint									m_iVertShAttr = 0;			//  координата вершины
	GLuint									m_iTexCoordLoc = 0;			//	текстурная координгата
	GLuint									m_iColorLoc = 0;			//	цвет вершины
	GLuint									m_iSamplerLoc = 0;			//	номер текстуры
	GLuint									m_iProjMatrix = 0;			//	матрица преобразования кординат координгата
	GLuint									m_iModelMatrix = 0;			//	матрица преобразования кординат координгата
	GLuint									m_ibUseTexture = 0;			//  использовать текстуру или нет
	SOGLMatrix*								m_ProjMatrix = nullptr;
#endif
public :
	//
	const size_t	VertexDimension = VertexDim;
	const size_t	ColorDimension = ColorDim;
	const size_t	TextureDimension = 2;
	//
	
	//
	COGLDrawBuffer() {
#ifdef OPENGL_SHADER
		glhLoadIdentityf2(&m_ModelMatrix);
#endif
	};
	void SetShader(Shader*	Shader) {
		m_Shader = Shader;
		auto	iprogramm = m_Shader->getShader();
		m_iVertShAttr = glGetAttribLocation(iprogramm, "vPosition");
		m_iTexCoordLoc = glGetAttribLocation(iprogramm, "vtexCoord");
		m_iColorLoc = glGetAttribLocation(iprogramm, "vColor");
		// Get the sampler location
		m_iSamplerLoc = glGetUniformLocation(iprogramm, "s_texture");
		//
		m_iProjMatrix = glGetUniformLocation(iprogramm, "u_ProjMatrix");
		m_iModelMatrix = glGetUniformLocation(iprogramm, "u_ModelMatrix");
		m_ibUseTexture = glGetUniformLocation(iprogramm, "b_UseTexture");
	};
	void SetProjectMatrix(SOGLMatrix*	ProjMatrix) { m_ProjMatrix = ProjMatrix;  };
	//
	//inline size_t  Add( GLfloat	U, GLfloat	V, GLfloat X, GLfloat  Y, GLfloat R, GLfloat G, GLfloat  B, GLfloat A ) {
	//	auto tSize = m_vTexture.Add(U, V);
	//	auto vSize = m_vVertex.Add(X, Y);
	//	assert(tSize == vSize);
	//	return vSize;
	//};

	//inline size_t  Add(GLfloat	U, GLfloat	V, GLfloat X, GLfloat  Y, GLfloat  Z) {
	//	auto tSize = m_vTexture.Add(U, V);
	//	auto vSize = m_vVertex.Add(X, Y, Z );
	//	assert(tSize == vSize);
	//	return vSize;
	//};

	//inline size_t  Add(GLfloat	U, GLfloat	V, GLfloat X, GLfloat  Y, GLfloat  Z,  GLfloat  W) {
	//	auto tSize = m_vTexture.Add(U, V);
	//	auto vSize = m_vVertex.Add(X, Y, Z, W );
	//	assert(tSize == vSize);
	//	return vSize;
	//};
	//  добавить тексттурные координаты
	inline size_t  AddTexture(GLfloat	U, GLfloat	V) {
		timer_0	tt("Buffer Add");
		auto tSize = m_vTexture.Add(U, V);
		return tSize;
	};
	inline size_t  AddTexture(const GLfloat* Begin, const GLfloat* End) {
		timer_0	tt("Buffer Add");
		auto cSize = m_vTexture.Add(Begin, End);
		return cSize;
	};
	//template <typename... Float>
	//inline size_t  AddTexture(Float ... X) {
	//	timer_0	tt("Buffer Add");
	//	//assert(false);
	//	auto vSize = m_vTexture.Add(/*std::forward<Float>*/(X) ...);
	//	return vSize;
	//};
	//  вместо предыдущей, закоментированной функции
	inline size_t  AddTexture(std::vector<GLfloat>&& Data) {
		timer_0	tt("Buffer Add");

		auto cSize = m_vTexture.Add(std::forward<std::vector<GLfloat>>(Data));
		return cSize;
	};
	//
#ifdef OPENGL_SHADER
	void		LoadIdentity()  noexcept { glhLoadIdentityf2(&m_ModelMatrix); };
	void	AffineTransformation2D(const Vec3& Offset, GLfloat Angle, const Vec3& RotAxis, const Vec3& Scale) noexcept
	{
		glhLoadIdentityf2(&m_ModelMatrix);
		glhTranslatef2(&m_ModelMatrix, Offset.x, m_fMaxHeight - Offset.y, Offset.z);
		glhScalef2(&m_ModelMatrix, Scale.x, Scale.y, Scale.z);
		glhRotatef2(&m_ModelMatrix, Angle*M_PIF / 180.0f, RotAxis.x, RotAxis.y, RotAxis.z);

	}

	void	AffineTransformation2D(const Vec3&& Offset, GLfloat Angle, const Vec3&& RotAxis, const Vec3&& Scale) noexcept
	{
		glhLoadIdentityf2(&m_ModelMatrix);
		glhTranslatef2(&m_ModelMatrix, Offset.x, Offset.y, Offset.z);
		glhScalef2(&m_ModelMatrix, Scale.x, Scale.y, Scale.z);
		glhRotatef2(&m_ModelMatrix, Angle*M_PIF / 180.0f, RotAxis.x, RotAxis.y, RotAxis.z);
	}

	void	Transform(const Vec3&& Offset) noexcept {
		glhTranslatef2(&m_ModelMatrix, Offset.x, Offset.y, Offset.z);
	}

	void	Scale(const Vec3&& Scale) noexcept {
		glhScalef2(&m_ModelMatrix, Scale.x, Scale.y, Scale.z);
	}


	void	AddVertices(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const SOGLColor& Color) noexcept
	{
		AddVertex(Vertises, Vertises + VertSize);
		AddTexture(Texture, Texture + TexSize);
		SetColor(Color);
	}
	void	AddVerticesAndTransform(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const SOGLColor& Color) noexcept
	{
		GLfloat* verOut = new GLfloat[VertSize];
		for (auto i = 0; i < VertSize; i += VertexDimension)
			glhVec3TransformCoord(verOut + i, Vertises + i, &m_ModelMatrix);
		AddVertex(verOut, verOut + VertSize);
		AddTexture(Texture, Texture + TexSize);
		SetColor(Color);
		delete[] verOut;
	}

	void	AddVertices(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const unsigned int* Indexes, int IndSize, 
							const SOGLColor& Color) noexcept
	{
		/*GLfloat* verOut = new GLfloat[VertSize];
		for (auto i = 0; i < VertSize; i += VertexDimension)
			glhVec3TransformCoord(verOut + i, Vertises + i, &m_ModelMatrix);
		AddVertex(verOut, verOut + VertSize);*/
		AddVertex(Vertises, Vertises + VertSize);
		AddTexture(Texture, Texture + TexSize);
		AddIndex(Indexes, Indexes + IndSize);
		SetColor(Color);
		//delete[] verOut;
	}
#endif
	//  добавить координаты  вертексов
	inline size_t  AddVertex(GLfloat X, GLfloat Y) {
		auto vSize = m_vVertex.Add(X, Y);
		return vSize;
	};
	inline size_t  AddVertex(GLfloat X, GLfloat Y, GLfloat Z) {
		timer_0	tt("Buffer Add");

		auto vSize = m_vVertex.Add(X, Y, Z);
		return vSize;
	};
	inline size_t  AddVertex(GLfloat X, GLfloat Y, GLfloat Z, GLfloat W) {
		assert(0);
		auto vSize = m_vVertex.Add(X, Y, Z, W);
		return vSize;
	};
	//template <typename... Float>
	//inline size_t  AddVertex(Float ... X  ) {
	//	timer_0	tt("Buffer Add");
	//	//assert(false);
	//	auto vSize = m_vVertex.Add(/*std::forward<Float>*/(X) ...);
	//	return vSize;
	//};
	//  вместо предыдущей, закоментированной функции
	inline size_t  AddVertex( std::vector<GLfloat>&& Data) {
		timer_0	tt("Buffer Add");

		auto cSize = m_vVertex.Add(std::forward<std::vector<GLfloat>>(Data));
		return cSize;
	};

	inline size_t  AddVertex( const GLfloat* Begin, const  GLfloat* End) {
		timer_0	tt("Buffer Add");

		auto cSize = m_vVertex.Add(Begin, End);
		return cSize;
	};

	inline size_t  AddVertex( GLfloat* Begin,  GLfloat* End) {
		timer_0	tt("Buffer Add");
		auto cSize = m_vVertex.Add(Begin, End);
		return cSize;

	};
	//  добавить цвет
	inline size_t  AddColor(GLfloat R, GLfloat G, GLfloat B) {
		auto cSize = m_vColor.Add(R, G, B);
		return cSize;
	};

	inline size_t  AddColor(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
		auto cSize = m_vColor.Add( R, G, B, A );
		return cSize;
	};

	inline size_t  AddColor(const GLfloat* Begin, const GLfloat* End ) {
		auto cSize = m_vColor.Add(Begin, End);
		return cSize;
	};

	inline size_t  AddColor(const SOGLColor& Color) {
		return  m_vColor.Add(Color.r, Color.g, Color.b, Color.a);
	};

	//  Вызывать после того как заполнены остальные составляющие ( Vertex , Texture ). Добавляется столько элементов, чтобы размер буфера соответствовал размерам остальных
	inline size_t  SetColor(GLfloat R, GLfloat G, GLfloat B) {
		auto count = m_vVertex.Count();
		size_t cSize;
		while( ( cSize = m_vColor.Add(R, G, B) ) < count );
		return cSize;
	};
	inline size_t  SetColor(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
		timer_0	tt("Buffer Add");
		auto count = m_vVertex.Count();
		size_t cSize;
		while ((cSize = m_vColor.Add(R, G, B, A)) < count);
		return cSize;
	};
	inline size_t  SetColor( const SOGLColor& Color ) {
		return  SetColor(Color.r, Color.g, Color.b, Color.a);
	};
	//

	size_t AddIndex(unsigned int Index) {
		auto cSize = m_vIndex.Add(Index);
		return cSize;
	};

	template< typename...Args>
	size_t AddIndex(unsigned int Index, Args...arg) {
		m_vIndex.Add(Index);
		auto cSize = AddIndex(arg...);
		return cSize;
	};

	inline size_t  AddIndex( std::vector<unsigned int>&& Data ) {
		auto cSize = m_vIndex.Add( std::forward<std::vector<unsigned int>>(Data));
		return cSize;
	};
	
	inline size_t  AddIndex(const unsigned int* Begin, const unsigned int* End) {
		auto cSize = m_vIndex.Add(Begin, End);
		return cSize;
	};
	//  проверка что размер всех буферов совпадает
	inline void Check() const noexcept {
		assert( ( m_vTexture.Count() == m_vVertex.Count() || m_vTexture.Count() == 0 ) && 
			( m_vVertex.Count() == m_vColor.Count() || m_vColor.Count() == 0) );
	};
	//
	inline void	Clear() noexcept {
		m_vVertex.Clear(); m_vTexture.Clear(); m_vColor.Clear(); m_vIndex.Clear();
	};
	//
	inline size_t	Count() const noexcept { 
		Check();
		return m_vVertex.Count();
	};
	inline size_t	VertexSize() const noexcept {
		return m_vVertex.Size();
	};
	inline size_t	TextureSize() const noexcept {
		return m_vTexture.Size();
	};
	inline size_t	ColorSize() const noexcept {
		return m_vColor.Size();
	};
	inline size_t	IndexSize() const noexcept {
		return m_vIndex.Size();
	};
	//
	inline const GLfloat*  GetVertexData() { return m_vVertex.GetData(); };
	inline const GLfloat*  GetTextureData() { return m_vTexture.GetData(); };
	inline const GLfloat*  GetColorData() { return m_vColor.GetData(); };
	inline const unsigned int*  GetIndexData() { return m_vIndex.GetData(); };

//	inline void SetLineWidth(unsigned int Width) { m_iLineWidth = Width; };
//	inline void SetPrimitive(unsigned int Primitive) { m_Primitive = Primitive; };
#ifndef OPENGLES20
	inline void Bind() {
		timer_0	tt("Buffer Bind");
		auto size = Count();
		if (VertexSize() != 0) {
			glGenBuffers(1, &m_iVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
			glBufferData(GL_ARRAY_BUFFER, VertexSize() * sizeof(GLfloat), GetVertexData(), GL_STATIC_DRAW);
			glVertexPointer(VertexDimension, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_VERTEX_ARRAY);
		}

		if (TextureSize() != 0) {
			glEnable(GL_TEXTURE_2D);
			glGenBuffers(1, &m_iTBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_iTBO);
			glBufferData(GL_ARRAY_BUFFER, TextureSize() * sizeof(GLfloat), GetTextureData(), GL_STATIC_DRAW);
			glTexCoordPointer(TextureDimension, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		else glDisable(GL_TEXTURE_2D);

		if (ColorSize() != 0) {
			glGenBuffers(1, &m_iCBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_iCBO);
			glBufferData(GL_ARRAY_BUFFER, ColorSize() * sizeof(GLfloat), GetColorData(), GL_STATIC_DRAW);
			glColorPointer(ColorDimension, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_COLOR_ARRAY);
		}
		
	}

	inline void UnBind() {
		timer_0	tt("Buffer UnBind");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (VertexSize() != 0) {
			glDeleteBuffers(1, &m_iVBO);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (TextureSize() != 0) {
			glDeleteBuffers(1, &m_iTBO);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}
		if (ColorSize() != 0) {
			glDeleteBuffers(1, &m_iCBO);
			glDisableClientState(GL_COLOR_ARRAY);
		}

		Clear();
	}
	//
	inline void DrawArray(unsigned int	Primitive) {
		Bind();
		glDrawArrays(Primitive, 0, Count());
		UnBind();
	}
	//
	inline void DrawElements(unsigned int	Primitive) {
		Bind();
		glDrawElements(	Primitive,			// режим
						IndexSize(),		// количество
						GL_UNSIGNED_INT,	// тип
						GetIndexData()		// смещение в элементном буфере
					);
		UnBind();
	}
#endif
	//
	void DrawBuffer(CGLTexture* Texture, unsigned int	Primitive) noexcept
	{
		timer_0	tt("Buffer FlushES");
		if (VertexSize() == 0)
			return;
		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		bool bindText = Texture->Bind();
		int useTexture = bindText && m_vTexture.Size() != 0;
		
		//
		glUseProgram(m_Shader->getShader());
		auto err = glGetError();
		// Load the vertex position
		glVertexAttribPointer(m_iVertShAttr, VertexDimension, GL_FLOAT, GL_FALSE, 0, m_vVertex.GetData());
		err = glGetError();
		// Load the texture coordinate
		glVertexAttribPointer(m_iTexCoordLoc, TextureDimension, GL_FLOAT, GL_FALSE, 0, m_vTexture.GetData());
		err = glGetError();
		// Load the texture coordinate
		glVertexAttribPointer(m_iColorLoc, ColorDimension, GL_FLOAT, GL_FALSE, 0, m_vColor.GetData());
		err = glGetError();
		//
		glUniform1i(m_ibUseTexture, useTexture);
			//
		glEnableVertexAttribArray(m_iVertShAttr);
		glEnableVertexAttribArray(m_iTexCoordLoc);
		glEnableVertexAttribArray(m_iColorLoc);

		

		// Set the texture sampler to texture unit to 0
		glUniform1i(m_iSamplerLoc, 0);
		glUniformMatrix4fv(m_iProjMatrix, 1, false, &(*m_ProjMatrix)._11);
		glUniformMatrix4fv(m_iModelMatrix, 1, false, &m_ModelMatrix._11);
		// Draw quad with nearest sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
		if (m_vIndex.Size() == 0)
			glDrawArrays(Primitive/*GL_TRIANGLES*/, 0, Count());
		else {
			glDrawElements(Primitive/*GL_TRIANGLES*/,			// режим
				m_vIndex.Size(),					// количество
				GL_UNSIGNED_INT,					// тип
				m_vIndex.GetData()					// смещение в элементном буфере
			);
			auto err = glGetError();
			if (err)
				int f = 99;
		}

		glDisableVertexAttribArray(m_iVertShAttr);
		glDisableVertexAttribArray(m_iTexCoordLoc);
		glDisableVertexAttribArray(m_iColorLoc);

		glDeleteProgram(m_Shader->getShader());

		Clear();
	};
	//
	void	Trace() {
		auto iv = 0, it = 0, ic = 0;
		const float* vert = m_vVertex.GetData();
		const float* tex = m_vTexture.GetData();
		const float* col = m_vColor.GetData();
		const unsigned int* ind = m_vIndex.GetData();
		for (auto i = 0; i != Count(); ++i, iv += VertexDimension, it += TextureDimension, ic += ColorDimension ) {
			IO_TRACE(" %d V { ", i);
			for( auto ii = 0 ; ii != VertexDimension ; ++ii )  IO_TRACE("%f, ", *(vert+iv+ii));
			IO_TRACE(" }  T { ");
			for (auto ii = 0; ii != TextureDimension; ++ii)  IO_TRACE("%f, ", *(tex + it + ii));
			IO_TRACE(" } C { ");
			for (auto ii = 0; ii != ColorDimension; ++ii)  IO_TRACE("%f, ", *(col + ic + ii));
			IO_TRACE("}\n");
		}
		IO_TRACE(" Ind { ");
		for (auto ii = 0; ii != m_vIndex.Size(); ++ii) IO_TRACE(" %u, ", *(ind+ii));
		IO_TRACE(" }\n ");
	}

};

using CTextVertexColorBuffer = COGLDrawBuffer< 3, 4, 10000 >;
