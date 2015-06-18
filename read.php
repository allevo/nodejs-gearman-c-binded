<?php

if (!class_exists('GearmanWorker')) {
	echo 'GearmanWorker class unknown';
	exit(1);
}

$worker = new GearmanWorker();
$worker->addServer('127.0.0.1', 4731);

function p($job) {
	echo json_encode(array(
		'func' => 'queue',
		'handler' => $job->handle(),
		'workload' => $job->workload(),
		'unique' => $job->unique(),
	));
}

$worker->addFunction($argv[1], 'p');
$worker->setTimeout(100);
while($worker->work());
