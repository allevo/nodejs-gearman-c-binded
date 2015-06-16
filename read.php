<?php


$worker = new GearmanWorker();
$worker->addServer('127.0.0.1', 4730);

function p($job) {
	var_dump($job->handle(), $job->workload());
}

$worker->addFunction('queue', 'p');

$worker->work();
