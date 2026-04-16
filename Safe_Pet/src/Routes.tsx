import React from 'react';
import {BrowserRouter, Routes, Route} from 'react-router-dom';
import Dashboard from './pages/Dashboard.tsx';
import Mapas from './pages/mapas.tsx';
import Saude from './pages/saude.tsx';
import Conectividade from './pages/conectividade.tsx';
import Configuracoes from './pages/configuracoes.tsx';
import './styles/Dashboard.css';

export default function Routing(){

	return(
		
		<BrowserRouter>
    			<Routes>
				<Route path="/" element={<Dashboard />} />
				<Route path="/dashboard" element={<Dashboard />} />
				<Route path="/mapas" element={<Mapas />} />
				<Route path="/saude" element={<Saude />} />
				<Route path="/conectividade" element={<Conectividade />} />
				<Route path="/configuracoes" element={<Configuracoes />} />
			</Routes>
		</BrowserRouter>
		

	);
}
