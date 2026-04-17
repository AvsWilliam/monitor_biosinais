import {BrowserRouter, Routes} from 'react-router-dom';
import App from './app.tsx';
import Dasboard from './pages/dasboard.tsx'


function Routing(){

	return(
		<BrowserRouter>
    			<Routes>
				<Route path='/' element={Dasboard}></Route>
			</Routes>
    			
		</BrowserRouter>

);
}