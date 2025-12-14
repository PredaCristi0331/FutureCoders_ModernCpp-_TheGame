import ChatMessage;

int chat::ChatMessage::GetId() const { 
	return m_id; 
}
void chat::ChatMessage::SetId(int id) {
	m_id = id;
}

int chat::ChatMessage::GetPlayerId() const {
	return m_playerId; 
}
void chat::ChatMessage::SetPlayerId(int id) {
	m_playerId = id; 
}

int chat::ChatMessage::GetGameId() const {
	return m_gameId; 
}
void chat::ChatMessage::SetGameId(int id) {
	m_gameId = id; 
}

const std::string& chat::ChatMessage::GetText() const {
	return m_text; 
}
void chat::ChatMessage::SetText(const std::string& text) {
	m_text = text; 
}

const std::string& chat::ChatMessage::GetTimestamp() const {
	return m_timestamp; 
}
void chat::ChatMessage::SetTimestamp(const std::string& ts) {
	m_timestamp = ts; 
}