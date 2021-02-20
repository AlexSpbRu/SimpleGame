#pragma once


#include <set>
#include <vector>


template< class T >
class Singleton 
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() = default;
	Singleton(const Singleton &) = delete;
	Singleton &operator=(const Singleton &) = delete;

};

/*  http://itnotesblog.ru/note.php?id=144#sthash.6CCapUr2.xZgccm40.dpbs    */


/*  Базовый класс  для источника событий.
	Однопоточный.
	Решена проблема подписки-отписки наблюдателя внутри обработчика событий ( наблюдателя ).
*/


template< class Observer >
class Observable {
public:
	virtual ~Observable() {
	}

	void registerObserver(Observer* observer) {
		if (m_count != 0) {
			m_requests.push_back(ObserverRequest{ &Observable< Observer >::registerObserver, observer });
		}
		else if (observer) {
			m_observers.insert(observer);
		}
	}

	void unregisterObserver(Observer* observer) {
		if (m_count != 0) {
			m_requests.push_back(ObserverRequest{ &Observable< Observer >::unregisterObserver, observer });
		}
		else if (observer) {
			m_observers.erase(observer);
		}
	}

protected:
	Observable() : m_count(0) {
	}

	template< typename F, typename... Args >
	void notifyObservers(F f, Args... args) {
		++m_count;
		for (Observer* o : m_observers) {
			(o->*f)(args...);
		};
		--m_count;
		if (m_count == 0) {
			for (const ObserverRequest& r : m_requests) {
				(this->*r.operation)(r.observer);
			};
			m_requests.clear();
		}
	}

private:
	struct ObserverRequest {
		void (Observable< Observer >::*operation)(Observer*);
		Observer* observer;
	};

	std::set< Observer* > m_observers;
	int m_count;   
	std::vector< ObserverRequest > m_requests;
};

//  Базовый класс для наблюдателя за контроллами типа кнопки.

//template < typename Button >
//class  IButtonEvent {
//public :
//	virtual		~IButtonEvent() {};
//	virtual		void	onButtonDown( const Button* Control )  noexcept = 0;
//	virtual		void	onButtonUp( const Button* Control )  noexcept = 0;
//};

class COGLControl;
class COGLButton;

class  IControlEvent {
public:
	virtual		~IControlEvent() {};
	virtual		void	onLButtonDown(const COGLControl* Control)  noexcept = 0;
	virtual		void	onLButtonUp(const COGLControl* Control)  noexcept = 0;
	virtual		void	onMouseMove(const COGLControl* Control)  noexcept = 0;
	virtual		void	onMouseLeave(const COGLControl* Control)  noexcept = 0;
	//
	virtual		void	onButtonDown(const COGLButton* Control)  noexcept = 0;
	virtual		void	onButtonUp(const COGLButton* Control)  noexcept = 0;
};

///  Базовый класс для наблюдателя за событиями клавиатуры 

class IKeyboardEvent {
public : 
	virtual		~IKeyboardEvent() {};
	virtual		void	onKeyDown( char Key )  noexcept  = 0;
	virtual		void	onKeyUp(char Key)  noexcept = 0;
};

///  Базовый класс для наблюдателя за событиями от мыши 

class IMouseEvent {
public:
	virtual		~IMouseEvent() {};
	virtual		void	onMouseMove( int X, int Y )  noexcept = 0;
	virtual		void	onLMouseDown(int X, int Y)  noexcept = 0;
	virtual		void	onLMouseUp(int X, int Y)  noexcept = 0;
};

///  Базовый класс для наблюдателя за событем - изменение размера экрана

class IResizeWindow {
public:
	virtual		~IResizeWindow() {};
	virtual		void	onResizeWindow(float Width, float Height )  noexcept = 0;
};

//-------------------------------------------------------------------------------------
//  Классы источники событий от клавиатуры и мыши.

class CMouseNotifier : public Observable< IMouseEvent >, public Singleton< CMouseNotifier >
{
public:
	void onMouseMove(int X, int Y) {
		notifyObservers(&IMouseEvent::onMouseMove, X, Y);
	};
	void onLMouseDown(int X, int Y) {
		notifyObservers(&IMouseEvent::onLMouseDown, X, Y);
	};
	void onLMouseUp(int X, int Y) {
		notifyObservers(&IMouseEvent::onLMouseUp, X, Y);
	};
};


class CKeyboardNotifier : public Observable< IKeyboardEvent >, public Singleton< CKeyboardNotifier >
{
public:
	void onKeyDown(char Key) {
		notifyObservers(&IKeyboardEvent::onKeyDown, Key);
	};
	void onKeyUp(char Key) {
		notifyObservers(&IKeyboardEvent::onKeyUp, Key);
	};
};

//-------------------------------------------------------------------------------------
//  Классы источники события - изменение размера экрана

class CWindowResizeNotifier : public Observable< IResizeWindow >, public Singleton< CWindowResizeNotifier >
{
public :
	void onResizeWindow(float Width, float Height){
		notifyObservers( &IResizeWindow::onResizeWindow, Width, Height );
	};
};