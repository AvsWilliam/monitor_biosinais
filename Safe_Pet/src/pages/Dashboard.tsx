import React from 'react';
import '../styles/Dashboard.css';
import {useNavigate} from 'react-router-dom'


export default function Dashboard(){

	const navigate = useNavigate();
				
	function go_to_dashboard(){
     		navigate('/dashboard');
	}

	function go_to_mapas(){
     		navigate('/mapas');
	}

	function go_to_saude(){
     		navigate('/saude');
	}

	function go_to_conectividade(){
     		navigate('/conectividade');
	}
	function go_to_configuracoes(){
     		navigate('/configuracoes');
	}

	return(

		<div className="corpo">
			<aside className="aside">
				<section className="option_button" onClick = {go_to_dashboard}>
					<p>Dashboard</p>
  				</section>

				<section className="option_button" onClick = {go_to_mapas}>
					<p>Mapas</p>
  				</section>

				<section className="option_button" onClick = {go_to_saude}>
					<p>Saúde</p>
  				</section>

				<section className="option_button" onClick = {go_to_conectividade}>
					<p>Conectividade</p>
  				</section>

				<section className="option_button" onClick = {go_to_configuracoes}>
					<p>Configurações</p>
  				</section>
	
  			</aside>


			<section className="main">

				<header className="header">
					<h2>Bem-Vindo!</h2>
  				</header>

				<section className="content">
					<p>main</p>
  				</section>
  			</section>
		</div>

    			
);
}
