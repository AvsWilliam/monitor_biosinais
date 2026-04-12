import React from 'react';
import {BrowserRouter, Routes, Route} from 'react-router-dom';
import Dashboard from './pages/Dashboard.tsx';
import './styles/Dashboard.css';

export default function Routing(){

	return(
		
		<BrowserRouter>
    			<Routes>
				<Route path="/" element={<Dashboard />} />
			</Routes>
		</BrowserRouter>
		

	);
}