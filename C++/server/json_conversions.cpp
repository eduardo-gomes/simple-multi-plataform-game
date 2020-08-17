#include "json_conversions.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

playerSetMsg::playerSetMsg() = default;
playerSetMsg::playerSetMsg(const player &p) {
	id = p.getId();
	name = p.getName();
	position = p.getPos();
	score = p.getScore();
}

void to_json(json &j, const playerSetMsg &p) {
	const std::string &pName = p.name, &pId = p.id;
	const pos &pPos = p.position;
	const unsigned int &pScore = p.score;
	j = json{
		{pId, {
			{"name", pName},
			{"score", pScore},
			{"pos", {
				{"x", pPos.x},
				{"y", pPos.y}
			}}
		}}
	};
}

void from_json(const json &j, playerSetMsg &p) {
	std::string &pName = p.name, &pId = p.id;
	pos &pPos = p.position;
	unsigned int &pScore = p.score;

	const json &jPlayer = j.front();
	const json &jPos = jPlayer.at("pos");

	pId = j.items().begin().key();

	jPlayer.at("name").get_to(pName);
	jPlayer.at("score").get_to(pScore);
	jPos.at("x").get_to(pPos.x);
	jPos.at("y").get_to(pPos.y);
}

std::string toJsonToString(const playerSetMsg &player) {
	json tmp = player;
	std::string out = tmp.dump();
	return out;
}

playerSetMsg fromJsonToString(const std::string &str) {
	json tmp = json::parse(str);
	playerSetMsg tmpret;
	tmp.get_to(tmpret);
	return tmpret;
}